<!-- +--------+
|Tinx 1.0|
+--------+ -->

# Tinx

Tinx is a simple HTTP server project implemented in C. It aims to provide a lightweight and easy-to-use solution for serving static files over the HTTP protocol.

# Features

- **Basic HTTP Server**: Tinx allows users to run a basic HTTP server on their local machine. It listens for incoming connections on a specified port and serves static files to clients.
  
- **File Serving**: Users can specify a directory from which the server serves files. When clients make requests for files, such as HTML, CSS, JavaScript, or image files, Tinx retrieves and sends them to the clients.

# Usage

1. **Compile the Server**: Compile the server code using a C compiler such as GCC:
```
gcc server.c -o server
```

2. **Run the Server**: Execute the compiled server binary:
```
./server
```
By default, Tinx listens on port 8080 and serves files from the specified directory.

3. **Access Files**: Access files served by Tinx using a web browser or command-line tool like cURL:
```
curl http://localhost:8080/index.html
```

# Project Structure

- `server.c`: Contains the source code for the Tinx HTTP server.
- `README`: Provides instructions and information about the Tinx project.

# Requirements

- C compiler (e.g., GCC)
- Linux environment (the code may require modifications to run on other platforms)

# Troubleshooting

- If the server fails to bind to the port, ensure that the port is not already in use by another application.
- If the server fails to serve files, double-check the file paths and permissions.

# Contributing

Contributions to Tinx are welcome! Feel free to submit bug reports, feature requests, or pull requests on the [GitHub repository](https://github.com/xanmoy/tinx).

# License

Tinx is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
