# Use an official Python runtime as a parent image
FROM amd64/ubuntu

# Set the working directory to /app
RUN mkdir /app
RUN apt update && apt -y install build-essential
# Copy the current directory contents into the container at /app
COPY . /app

# Run app.py when the container launches
CMD ["/app/server"]


