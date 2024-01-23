# SystemC Docker Image Setup

This README provides instructions on setting up a SystemC development environment using a Docker image. The Docker image includes Ubuntu 20.04, g++ 9.3.0, gdb 9.1, make, vim, and SystemC 2.3.3. Follow the steps below to get started on Windows, Mac OS, or Linux.

## Prerequisites

- [Docker Desktop](https://www.docker.com/products/docker-desktop) installed on your system.

## Usage Instructions

1. **Download and Install Docker Desktop:**

   Make sure to install Docker Desktop on your machine. You can download it from [here](https://www.docker.com/products/docker-desktop) and follow the installation instructions for your operating system.

2. **Open a Terminal:**

   Open a terminal on your machine.

3. **Download SystemC Docker Image:**

   Run the following command to download the SystemC Docker image from Docker Hub:

   ```bash
   docker pull learnwithexamples/systemc
   ```

4. **Run the Docker Image:**

   Start a Docker container using the downloaded image with the following command:

   ```bash
   docker run -it learnwithexamples/systemc
   ```

   This will launch a new shell inside the Docker container.

5. **Verify Environment Setup:**

   A "hello world" example is provided in the Docker image to verify the environment settings. Navigate to the example directory and build/run the code:

   ```bash
   cd helloworld
   make
   ./out
   ```

   If everything is set up correctly, you should see the output of the "hello world" program.

6. **Modify or Create Your Own Code:**

   You can now modify the `helloworld.cpp` file using the included Vim editor, or create your own C++ source code within the Docker container.

   ```bash
   vim helloworld.cpp
   ```

   After making changes, you can use the provided Makefile to build and run your code.

7. **Exit the Docker Container:**

   Once you're done working within the Docker container, you can exit the container by typing:

   ```bash
   exit
   ```

   This will return you to your host machine's terminal.

That's it! You've successfully set up a SystemC development environment using Docker. Feel free to customize the environment further based on your specific project requirements.
