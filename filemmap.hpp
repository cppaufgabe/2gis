#include <filesystem>
#include <unistd.h>
#include <fcntl.h>
#include <span>
#include <sys/mman.h>
#include <sys/stat.h>
#include <vector>
#include <thread>

// https://github.com/HappyCerberus/1brc


template <typename T, T empty = T{}>
struct MoveOnly
{
    MoveOnly() : store_(empty) {}
    MoveOnly(T value) : store_(value) {}
    MoveOnly(MoveOnly &&other)
        : store_(std::exchange(other.store_, empty)) {}
    MoveOnly &operator=(MoveOnly &&other)
    {
        store_ = std::exchange(other.store_, empty);
        return *this;
    }
    operator T() const { return store_; }
    T get() const { return store_; }

private:
    T store_;
};

struct FileFD
{
    FileFD(const std::filesystem::path &file_path)
        : fd_(open(file_path.c_str(), O_RDONLY))
    {
        if (fd_ == -1)
            throw std::system_error(errno, std::system_category(), "Failed to open file");
    }
    ~FileFD()
    {
        if (fd_ >= 0)
            close(fd_);
    }
    int get() const { return fd_.get(); }

private:
    MoveOnly<int, -1> fd_;
};

struct MappedFile
{
    MappedFile(const std::filesystem::path &file_path)
        : fd_(file_path)
    {
        // Determine the filesize (needed for mmap)
        struct stat sb;
        if (fstat(fd_.get(), &sb) == 1) {
            throw std::system_error(errno, std::system_category(), "Failed to read file stats");
        }
        sz_ = sb.st_size;
        begin_ = static_cast<char *>(mmap(NULL, sz_, PROT_READ, MAP_PRIVATE, fd_.get(), 0));
        if (begin_ == MAP_FAILED)
        {
            throw std::system_error(errno, std::system_category(), "Failed to map file to memory");
        }
        chunk_begin_ = begin_;
    }

    ~MappedFile()
    {
        if (begin_ != nullptr)
            munmap(begin_, sz_);
    }
    // the entire file content as a std::span
    std::span<const char> data() const
    {
        return {begin_.get(), sz_.get()};
    }

    std::vector<std::span<const char>> chunked(size_t chunks) const
    {
        std::vector<std::span<const char>> result;

        size_t chunk_sz = sz_ / chunks;
        const char *chunk_begin = begin_;
        for (size_t i = 0; i < chunks - 1; ++i)
        {
            auto end = chunk_begin + chunk_sz;
            // align the end of the chunk to '\n' character
            while (end != begin_ + sz_ && *end != '\n')
                ++end;
            ++end;
            result.push_back({chunk_begin, end});
            chunk_begin = end;
        }
        // Manually set the last chunk
        result.push_back({chunk_begin, begin_ + sz_});

        return result;
    }

    std::span<const char> next_chunk()
    {
        std::lock_guard lock{mux_};
        if (chunk_begin_ == begin_ + sz_)
            return {};

        size_t chunk_sz = 64 * 1024 * 1024; // 64MB

        const char *end = nullptr;
        // prevent reading past the end of the file
        if (chunk_begin_ + chunk_sz > begin_ + sz_)
        {
            end = begin_ + sz_;
        }
        else
        {
            end = chunk_begin_ + chunk_sz;
            while (end != begin_ + sz_ && *end != '\n')
            {
                ++end;
            }
            ++end;
        }
        std::span<const char> result{chunk_begin_, end};
        chunk_begin_ = end;
        return result;
    }

private:
    FileFD fd_;
    MoveOnly<char *> begin_;
    const char *chunk_begin_;
    MoveOnly<size_t> sz_;
    std::mutex mux_;
};