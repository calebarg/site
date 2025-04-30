#### NOTE(calebarg): This is just as much for you as it is for anyone else reading this.

### Need zig 0.13.0 compiler.

### Steps

- Add/modify html/css file(s)
- Run/compile the site generator ./build_linux.sh
- Copy changed files to s3 (aws s3 cp <file> s3://calebarg.net/<file>)
- Run deploy.sh script

