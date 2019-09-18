#!/usr/bin/env python3

import os
from cmd import Cmd

import common
import take_photo
import get_server_state
import start_recording
import stop_recording
import start_timelapse
import open_webcam
import clear_data
import start_monitor
import get_version
import list_files
import take_3
import automated_timelapse
import download_all
import set_rotation


class MyPrompt(Cmd):
    def __init__(self, service, file_server):
        super().__init__(completekey='tab')
        self._service = service
        self._file_server = file_server

    def default(self, input):
        print("Unsupported command.")
        self.do_help("")

    def do_exit(self, input):
        'Exits the program'
        print("Exiting")
        return True

    def do_set_rotation(self, input):
        '''set_rotation $degrees=180.

        Set the rotation of the camera. If the rotation is not 0 degrees, the server will rotate the
        images before they are consumed.
        '''
        set_rotation.set_rotation(self._service, *parse_int(input))

    def do_open_webcam(self, input):
        '''open_webcam $webcam_id=1

        Sets the server to the specified webcam id (as listed in /dev/video{0,1,2,etc}).
        '''
        open_webcam.open_webcam(self._service, *parse_int(input))

    def do_automated_timelapse(self, input):
        '''fnname args

        TODO(curtismuntz): add help
        '''
        automated_timelapse.automated_timelapse(self._service)

    def do_clear_data(self, input):
        '''clear_data

        Clears the server's data directory.
        '''
        clear_data.clear_data(self._service)

    def do_download_all(self, input):
        '''download_all

        Downloads all the supported media files from the fileserver into the specified destination
        directory.
        '''
        # TODO(curtismuntz): support parameter passing here
        download_all.download_all(self._service, self._file_server)

    def do_get_server_state(self, input):
        '''get_server_state

        Prints the state of the server (Active, Idle)
        '''
        get_server_state.get_server_state(self._service)

    def do_get_version(self, input):
        '''get_version

        Prints the version of witness running on the server
        '''
        get_version.get_version(self._service)

    def do_list_files(self, input):
        '''list_files

        Lists the supported media files in the data directory of the server
        '''
        list_files.list_files(self._service)

    def do_start_monitor(self, input):
        '''start_monitor

        Starts the monitoring service
        '''
        start_monitor.start_monitor(self._service)

    def do_start_recording(self, input):
        '''start_recording

        Starts the recording service with the given filename. If the filename is blank, one will be
        generated.
        '''
        # TODO(curtismuntz): support parameter passing here
        start_recording.start_recording(self._service)

    def do_start_timelapse(self, input):
        '''start_timelapse

        Starts a timelapse service with the specified sleep time between frames and specified output
        filename. If no filename is given, one will be generated.
        '''
        # TODO(curtismuntz): support parameter passing here
        start_timelapse.start_timelapse(self._service)

    def do_stop_recording(self, input):
        '''stop_recording

        Stops any active service on the server (monitor, recording, timelapse, etc)
        '''
        stop_recording.stop_recording(self._service)

    def do_take_3(self, input):
        '''take_3

        Takes 3 photos sequentially with a 1 second sleep in between photos.
        These photos will have generated file names.
        '''
        take_3.take_3(self._service)

    def do_take_photo(self, input):
        '''take_photo

        Takes a photo with the specified filename. If no name is specified, one
        will be generated.
        '''
        # TODO(curtismuntz): support parameter passing here
        take_photo.take_photo(self._service)


def parse_int(arg):
    'Convert a series of zero or more numbers to an argument tuple'
    return tuple(map(int, arg.split()))


def main():
    arg_parser = common.create_arg_parser()
    grpc_server, file_server = common.services(arg_parser)

    service = common.create_service(grpc_server)

    p = MyPrompt(service, file_server)
    p.cmdloop()


if __name__ == "__main__":
    main()
