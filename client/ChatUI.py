from RestAPIWrapper.RestAPIWrapper import RestAPIWrapper
from WebsocketWrapper.WebsocketWrapper import WebsocketAPIWrapper

import dearpygui.dearpygui as dpg

from threading import Lock

class ChatUI:
    rest_api_client = None

    ws_api_client = None

    username = None

    mutex = None

    def __init__(self, rest_api_endpoint, ws_endpoint):
        self.rest_api_client = RestAPIWrapper(rest_api_endpoint)
        self.ws_api_client = WebsocketAPIWrapper(
            ws_endpoint=ws_endpoint, 
            on_open=self._WsOnOpen, 
            on_message=self._WsOnMessage, 
            on_close=self._WsOnClose, 
            on_error=self._WsOnError
        )
        self.mutex = Lock()

    def _ShowMainWindow(self):
        print("MAIN")
        dpg.delete_item("PRIMARY_WINDOW", children_only=True)
        dpg.add_text("MAIN WINDOW", parent="PRIMARY_WINDOW")
    
    def _WsOnOpen(self):
        print("OPEN")
        self._ShowMainWindow()
    
    def _WsOnClose(self):
        print("CLOSE")
        self._ShowInitWindow()
    
    def _WsOnError(self):
        print("ERROR")
        self._ShowInitWindow()
        #show modal window
    
    def _WsOnMessage(self, message):
        print("MSG")
        print(message)

    def _SetUsernameOpenWsConncetion(self, username):
        self.ws_api_client.SetUsernameHeader(username)
        self.ws_api_client.Run()

    def _ShowInitWindow(self):
        dpg.delete_item("PRIMARY_WINDOW", children_only=True)

        username_input = dpg.add_input_text(
            parent="PRIMARY_WINDOW",
            hint="Username",
            no_spaces=True,
            on_enter=True,
            callback=lambda s, a, u: self._SetUsernameOpenWsConncetion(a)
        )

        dpg.add_button(
            parent="PRIMARY_WINDOW",
            label="Join",
            callback=lambda s, a, u: self._SetUsernameOpenWsConncetion(dpg.get_value(username_input))
        )
    
    def _Run(self):
        dpg.create_context()
        dpg.create_viewport(title='Simple Chat', width=900, height=700)

        dpg.add_window(tag="PRIMARY_WINDOW")
        self._ShowInitWindow()

        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.set_primary_window("PRIMARY_WINDOW", True)

        dpg.start_dearpygui()

        dpg.destroy_context()
    
    def Start(self):
        self._Run()
