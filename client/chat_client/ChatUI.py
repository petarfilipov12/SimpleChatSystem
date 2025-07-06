from WebsocketWrapper.WebsocketWrapper import WebsocketAPIWrapper

import dearpygui.dearpygui as dpg

from threading import Lock

class ChatUI:
    ws_api_client = None
    username = None

    mutex = None

    def __init__(self, ws_endpoint):
        self.ws_api_client = WebsocketAPIWrapper(
            ws_endpoint=ws_endpoint, 
            on_open=self._WsOnOpen, 
            on_message=self._WsOnMessage, 
            on_close=self._WsOnClose, 
            on_error=self._WsOnError
        )
        self.mutex = Lock()
                
    def _ChatBoxCallback(self, sender, app_data, user_data):
        dpg.set_value("ChatBox", "")
        self.ws_api_client.Send(app_data)

    def _ShowMainWindow(self):
        dpg.add_text("Username: ", tag="UsernameText", parent="PRIMARY_WINDOW")
        with dpg.child_window(parent="PRIMARY_WINDOW", resizable_x=False, resizable_y=True, width=-1, height=500, border=True):
            dpg.add_text(tag="ChatWindowText")
        with dpg.child_window(parent="PRIMARY_WINDOW", resizable_x=False, resizable_y=False, width=-1, height=-1, border=True):
            dpg.add_input_text(tag="ChatBox", on_enter=True, callback=self._ChatBoxCallback)
    
    def _WsOnOpen(self):
        dpg.set_value("UsernameText", "Username: " + self.username)
        dpg.delete_item("USER_WINDOW")
    
    def _WsOnClose(self):
        self._ShowInitWindow()
    
    def _WsOnError(self):
        self._ShowInitWindow()
    
    def _WsOnMessage(self, message):
        self.mutex.acquire()
        dpg.configure_item("ChatWindowText",
            default_value=dpg.get_value("ChatWindowText") + "\n" + message
        )
        self.mutex.release()

    def _SetUsernameOpenWsConncetion(self, username):
        self.username=username
        self.ws_api_client.SetUsernameHeader(username)
        self.ws_api_client.Run()

    def _ShowInitWindow(self):
        if("USER_WINDOW" not in dpg.get_aliases()):
            with dpg.window(tag="USER_WINDOW", label="Log In", modal=True, no_collapse=True, no_close=True):
                username_input = dpg.add_input_text(
                    hint="Username",
                    no_spaces=True,
                    on_enter=True,
                    callback=lambda s, a, u: self._SetUsernameOpenWsConncetion(a)
                )

                dpg.add_button(
                    label="Join",
                    callback=lambda s, a, u: self._SetUsernameOpenWsConncetion(dpg.get_value(username_input))
                )
    
    def _Run(self):
        dpg.create_context()
        dpg.create_viewport(title='Simple Chat', width=900, height=700)

        dpg.add_window(tag="PRIMARY_WINDOW", on_close=lambda s, a, u: self.ws_api_client.Disconnect())
        self._ShowMainWindow()
        self._ShowInitWindow()

        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.set_primary_window("PRIMARY_WINDOW", True)

        dpg.start_dearpygui()

        dpg.destroy_context()
    
    def Start(self):
        self._Run()
