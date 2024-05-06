/*
 * server.c
 *
 * Tinx is a simple HTTP server project implemented in C.
 * It aims to provide a lightweight and easy-to-use solution for serving static files over the HTTP protocol.
 * Tinx allows users to run a basic HTTP server on their local machine.
 * It listens for incoming connections on a specified port and serves static files to clients.
 * Users can specify a directory from which the server serves files.
 * When clients make requests for files, such as HTML, CSS, JavaScript, or image files, Tinx retrieves and sends them to the clients.
 * This server is only compatible in Linux systems. to use this in other systems, you need to modify the code.
 *
 * copyright @ Tanmoy Ganguly
 */

#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

void send_response(int client_fd, const char *status, const char *content_type, off_t content_length)
{
    dprintf(client_fd, "HTTP/1.1 %s\r\n", status);
    dprintf(client_fd, "Content-Type: %s\r\n", content_type);
    dprintf(client_fd, "Content-Length: %ld\r\n", content_length);
    dprintf(client_fd, "\r\n");
}

int main()
{
    int s = socket(AF_INET, SOCK_STREAM, 0); // create a socket
    if (s == -1)
    {
        perror("Socket creation failed");
        return 1;
    }

    printf("Socket created.\n");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080), // port 8080
        .sin_addr.s_addr = INADDR_ANY}; // address to bind to

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    { // bind the socket to the address
        perror("Socket binding failed");
        close(s);
        return 1;
    }

    printf("Socket bound to port 8080.\n");

    if (listen(s, 10) == -1)
    { // listen for incoming connections
        perror("Listen failed");
        close(s);
        return 1;
    }

    printf("Server is listening for connections.\n");

    while (1)
    {
        int client_fd = accept(s, NULL, NULL); // accept a connection
        if (client_fd == -1)
        {
            perror("Accept failed");
            close(s);
            return 1;
        }

        printf("Connection accepted from client.\n");

        char buffer[BUFFER_SIZE];
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0); // receive data from the client
        if (bytes_received <= 0)
        {
            perror("Receive failed");
            close(client_fd);
            continue;
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received data

        // Extract file name from the request
        char *filename = strtok(buffer, " "); // Tokenize the request
        filename = strtok(NULL, " ");         // Get the second token (file path)
        if (filename == NULL || strcmp(filename, "/") == 0)
        {
            filename = "/index.html";
        }

        // Open the requested file
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "tinx%s", filename);
        int opened_fd = open("/home/xanmoy/Workspace/tinx/index.html", O_RDONLY); // Skip the leading "/"
        if (opened_fd == -1)
        {
            perror("File open failed");
            close(client_fd);
            continue;
        }

        printf("File opened successfully: %s\n", filepath);

        // Get file size
        struct stat st;
        if (fstat(opened_fd, &st) == -1)
        {
            perror("Fstat failed");
            close(opened_fd);
            close(client_fd);
            continue;
        }
        off_t file_size = st.st_size;

        // Send HTTP headers
        send_response(client_fd, "200 OK", "text/html", file_size);

        // Send the file content to the client
        off_t offset = 0;
        ssize_t bytes_sent = sendfile(client_fd, opened_fd, &offset, file_size);
        if (bytes_sent == -1)
        {
            perror("File send failed");
        }

        // Close file and client socket
        close(opened_fd);
        close(client_fd);
    }

    close(s); // close the server (will never reach here)
}
