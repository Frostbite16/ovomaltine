use std::{
    fs,
    io::{prelude::*, BufReader},
    net::{TcpListener, TcpStream}

};
fn main() {
    let listener = TcpListener::bind("127.0.0.1:7878").unwrap();

    for stream in listener.incoming(){
        let stream = stream.unwrap();

        println!("Connection established");
        http_handling(stream);
    }
}


fn http_handling(mut stream: TcpStream){
    let buf_reader = BufReader::new(&stream);
    let request_line = buf_reader.lines().next().unwrap().unwrap();

    if request_line == "GET / HTTP/1.1"{
        let status_line = "HTTP/1.1 200 OK";
        let contents = fs::read_to_string("/home/guilherme/github/ovomaltine/Rust/http-web-server/main.html").unwrap();
        let len = contents.len();

        let response = format!("{status_line}\r\nContents.length: {len}\r\n\r\n{contents}");
        

        stream.write_all(response.as_bytes()).unwrap();
    }
    else{
        let status_line = "HTTP/1.1 404 NOT FOUND";
        let contents = fs::read_to_string("/home/guilherme/github/ovomaltine/Rust/http-web-server/404.html").unwrap();
        let len = contents.len();

        let response = format!("{status_line}\r\nContents.length: {len}\r\n\r\n{contents}");

        stream.write_all(response.as_bytes()).unwrap();
    }

    
}
