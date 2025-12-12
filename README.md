# Web Server

A simple HTTP web server with LRU caching implemented in C++.

## Project Structure

```
root/
├── csrc/
│   ├── inc/
│   │   └── hash.h
├── ├── serverfiles/            # Server error pages
│   │   └── 404.html
├── ├── serverroot/             # Web content & source files
│   ├── ├── index.html
│   │   └── cat.jpg
│   ├── cache.h
│   ├── hashtable.h
│   ├── cache.cpp
│   ├── file.h
│   ├── file.cpp
│   ├── hashtable.cpp
│   ├── llist.h
│   ├── llist.cpp
│   ├── mime.h
│   ├── mime.cpp
│   ├── mime_table.cpp
│   └── net.h
│   └── net.cpp
└── server.cpp              # Main server file
```

## Features

- HTTP/1.1 GET request handling
- LRU cache for frequently accessed files
- MIME type detection for common file types
- 404 error page support
- Cross-platform (Windows/Linux/Mac)

## Building

### Windows
```batch
build.bat
```

### Linux/Mac
```bash
chmod +x build.sh
./build.sh
```

### Using Make
```bash
make
```

## Running

### Windows
```batch
webserver.exe
```

### Linux/Mac
```bash
./webserver
```

The server will start on port **3490**. Visit `http://localhost:3490` in your browser.

## Configuration

Edit `server.cpp` to change:
- `PORT` - Server port (default: 3490)
- `SERVER_ROOT` - Directory for web content (default: ./serverroot)
- `SERVER_FILES` - Directory for server files like 404.html (default: ./serverfiles)
- `CACHE_MAX_SIZE` - Maximum cached files (default: 10)
- `CACHE_HASHSIZE` - Cache hash table size (default: 128)

## Supported MIME Types

- HTML (.html, .htm)
- Images (.jpg, .jpeg, .png, .gif)
- CSS (.css)
- JavaScript (.js)
- JSON (.json)
- Text (.txt)

## Cache

The server uses an LRU (Least Recently Used) cache to store frequently accessed files in memory, improving performance for repeated requests.