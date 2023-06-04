# Use an official Ubuntu runtime as a parent image
FROM ubuntu:23.04

# Set the working directory to /app
WORKDIR /app

# Install any needed packages specified in requirements.txt
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libopencv-dev \
    libsane-dev

# Copy the current directory contents into the container at /app
COPY . /app

# Make port 80 available to the world outside this container
EXPOSE 80

# Define environment variable
ENV NAME World

# Run cmake when the container launches
RUN mkdir build
WORKDIR /app/build
RUN cmake ..
RUN make

# Run the output program from the previous step
CMD ["./scanner"]
