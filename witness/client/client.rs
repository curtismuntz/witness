// Copyright 2018 The Bazel Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

extern crate grpc;
extern crate rust_grpc;

use std::env;
use std::str::FromStr;

use rust_grpc::*;

fn parse_args() -> (String, u16) {
    let mut name = "world".to_owned();
    let mut port = 50051;
    for arg in env::args().skip(1) {
        if arg.starts_with("-p=") {
            port = u16::from_str(&arg[3..]).unwrap()
        } else {
            name = arg.to_owned();
        }
    }
    (name, port)
}

fn main() {
    let (name, port) = parse_args();
    println!("{:?}, {:?}", name, port);
    let client = WitnessCameraServiceClient::new_plain("::1", port, Default::default()).unwrap();
    let mut req = StartRecordingRequest::new();
    req.set_filename(name);
    let resp = client.start_recording(grpc::RequestOptions::new(), req);
    println!("{:?}", resp.wait());
}
