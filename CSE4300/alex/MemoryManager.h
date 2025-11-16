#include <vector>
#include <cstdint>

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

struct pageTableEntry {
    bool validBit = false;
    bool presentBit = false; // will implement page replacement later
    bool referenceBit = false;
    bool modifyBit = false;
    int pageFrameNum = -1;
};

class MemoryManager {
    private:
        std::vector<pageTableEntry> pageTable;
        std::vector<uint8_t> physicalMemory;
        std::vector<bool> freeFrames;

        std::vector<uint8_t> diskStorage; // just going to simulate disk storage with a vector

        int PAGE_SIZE; // 4096 bytes, 4K per page
        int PAGE_COUNT; // 1024 entries in the page table
        int PHYSICAL_SIZE; // # of bytes of physical memory

        int clockPointer = 0; // for CLOCK page replacement

        // initalizes vectors. internal use
        void _initializeMemory();

        // write data to physical address. internal use
        void _writeMemory(int physicalAddress, uint8_t data);
        // read data from physical address. returns data read  (uint8_t). internal use
        uint8_t _readMemory(int physicalAddress);

        // allocate a page. internal use
        void _allocatePage(int virtualPageNumber, int frameNumber);

        // translate virtual to physical address; handle page fault if data not present. returns physical address. internal use
        int _virtualToPhysicalAddress(int virtualAddress, bool writeOperation);

        // handle page fault by replacing and loading pages. internal use
        void _handlePageFault(int virtualPageNumber);
        // eject page frames using a CLOCK algorithm. returns new frame number. internal use
        int _replacePage();

        // set all data in a frame to 0. internal use
        void _wipeMemoryFrame(int frameNumber);

        // write pages frame to "disk". internal use
        void _writePageToDisk(int virtualPageNumber);
        // read pages frame from "disk" back to memory. internal use
        void _readPageFromDisk(int virtualPageNumber, int frameNumber);
        // erase pages data from disk. internal use
        void _deletePageFromDisk(int virtualPageNumber);

    public:
        // initalize memory manager with default parameters (4096 byte page size, 1024 PTEs, 1024 physical memory frames)
        MemoryManager();
        // initalize memory manager with custom parameters: Page Size (bytes), Page Table Entries, Physical Memory Frames
        MemoryManager(int page_size, int num_pages, int num_frames);

        // allocate a page in the table. will replace existing page if no free frames. returns virtual memory address
        int allocateAnyPage();

        // write to a virtual memory address
        void writeVirtualMemory(int virtualAddress, uint8_t data);
        // read from a virtual memory address. returns data (uint8_t)
        uint8_t readVirtualMemory(int virtualAddress);

        // delete a page table entry and free its memory/disk usage
        void deletePageTableEntry(int virtualAddress);

        // print stats for a page table entry at an address to std::cout
        void printPageTableEntry(int virtualAddress);
};

#endif
