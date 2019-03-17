#!/usr/bin/env python3

import os
import argparse
import tkinter as tk
import collections

import endpoint_scripts.common
import endpoint_scripts.take_photo
import endpoint_scripts.get_server_state
import endpoint_scripts.start_recording
import endpoint_scripts.stop_recording
import endpoint_scripts.start_timelapse
import endpoint_scripts.open_webcam
import endpoint_scripts.clear_data
import endpoint_scripts.start_monitor
import endpoint_scripts.get_version
import endpoint_scripts.list_files
import endpoint_scripts.take_3
import endpoint_scripts.automated_timelapse
import endpoint_scripts.download_all


def main():
    arg_parser = endpoint_scripts.common.create_arg_parser()
    grpc_server, file_server = endpoint_scripts.common.services(arg_parser)

    service = endpoint_scripts.common.create_service(grpc_server)
    endpoints = {
        "automated_timelapse":
        lambda: endpoint_scripts.automated_timelapse.automated_timelapse(service),
        "clear_data": lambda: endpoint_scripts.clear_data.clear_data(service),
        "download_all": lambda: endpoint_scripts.download_all.download_all(service, file_server),
        "get_server_state": lambda: endpoint_scripts.get_server_state.get_server_state(service),
        "get_version": lambda: endpoint_scripts.get_version.get_version(service),
        "list_files": lambda: endpoint_scripts.list_files.list_files(service),
        "open_webcam": lambda: endpoint_scripts.open_webcam.open_webcam(service),
        "set_rotation": lambda: endpoint_scripts.set_rotation.set_rotation(service),
        "start_monitor": lambda: endpoint_scripts.start_monitor.start_monitor(service),
        "start_recording": lambda: endpoint_scripts.start_recording.start_recording(service),
        "start_timelapse": lambda: endpoint_scripts.start_timelapse.start_timelapse(service),
        "stop_recording": lambda: endpoint_scripts.stop_recording.stop_recording(service),
        "take_3": lambda: endpoint_scripts.take_3.take_3(service),
        "take_photo": lambda: endpoint_scripts.take_photo.take_photo(service),
    }
    sorted_endpoints = collections.OrderedDict(sorted(endpoints.items()))

    root = tk.Tk()
    root.title(grpc_server)
    buttons = []

    for item in sorted_endpoints:
        button = tk.Button(root, text=item, command=sorted_endpoints[item])
        button.pack()
        buttons.append(button)

    # Window seem to need to be ~28 pixels tall per button
    window_height = str(len(list(sorted_endpoints.keys()) * 28))
    max_width = len(max(list(sorted_endpoints.keys()), key=len))
    # Roughly 7 pixels wide per character
    window_width = str((max_width * 7) + 7)
    window_size = window_width + "x" + window_height
    root.geometry(window_size)
    tk.mainloop()


if __name__ == "__main__":
    main()
