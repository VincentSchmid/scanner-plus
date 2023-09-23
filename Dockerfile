# Builder stage
FROM ubuntu:23.04 AS builder

# Set the working directory to /app
WORKDIR /app

# Install build dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libopencv-dev \
    libsane-dev

# Copy the current directory contents into the container at /app
COPY . /app

# Build the application
RUN mkdir build && cd build && cmake .. && make

# Final stage
FROM ubuntu:23.04

# Set the working directory to /app
WORKDIR /app

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libopencv-dev \
    libsane-dev

# Copy the built application from the builder stage
COPY --from=builder /app/build/scanner /app/

# Make port 80 available to the world outside this container
EXPOSE 80

# Define environment variable
ENV NAME World

# Command to run the application
CMD ["./scanner"]
