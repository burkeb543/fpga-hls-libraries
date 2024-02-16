# Microchip SmartHLS Libraries

Microchip currently provides two open source libraries that complement and expand the functionality of the SmartHLS tool: the [SmartHLS Vision Library](vision), and the [SmartHLS Fixed-Point Math Library](math). These libraries are currently in active development. To use the libraries, you should first install the SmartHLS tool, which is part of the [Libero SoC Design Suite](https://www.microchip.com/en-us/products/fpgas-and-plds/fpga-and-soc-design-tools/fpga/libero-software-later-versions).
 
  `Note: This release of the library is designed to work on the latest Libero SoC Design Suite version, 2024.1, and may not be compatible with previous versions of Libero and SmartHLS.`

A stable copy of the libraries is included with the SmartHLS installer under the `<PATH TO SMARTHLS INSTALLATION>/SmartHLS/smarthls-library/external` directory. The snapshot of the library is taken at the time when SmartHLS is released, while the Github version can include bug fixes and new functionality that may not be present in the installed copy.

## Setup

To use the latest version of the library, users should clone the library from Microchip's GitHub site:

- First, make sure you have Git and Git LFS installed. If not, you can follow these instructions: https://github.com/git-guides/install-git
  - The repository uses Git Large File System (LFS) to track media files (e.g., images).

- Then clone this SmartHLS library repository,
  - The following instructions use the command line to clone. You may use your prefered Git client instead.
  - We recommend cloning the repository to a directory path that is less than 80 characters long, e.g., `C:\Microchip\fpga-hls-libraries`.
    This is because Windows OS has a 256-character path length limit.
    Some files generated by Libero project deep down under the repository's directory structure can go over the path limit.
  - Here are the commands:
    ```console
    > cd <DIRECTORY TO CLONE THE LIBRARY INTO>
    > git lfs install
    > git clone https://github.com/MicrochipTech/fpga-hls-libraries.git
    > git lfs pull
    ```
    
- Next, update any existing designs that use the libraries to include the path where the libraries have been cloned. This can be done by adding `USER_CXX_FLAG+=-I/path/to/cloned/library` to your Makefile.

