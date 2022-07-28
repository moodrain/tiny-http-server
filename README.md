# tiny-http-server

* Tiny http server with multiple threads.
* Respond the Nth number of Fibonacci sequence accordingly, when received the query key named "num".


### Compilation
* install uriparser first. https://github.com/uriparser/uriparser
```
mkdir build && cd build && cmake .. && cmake --build .
```

### Run Server
```
./tiny-http-server
```

### Test
```
wget -qO- http://127.0.0.1:8080?num=10
```