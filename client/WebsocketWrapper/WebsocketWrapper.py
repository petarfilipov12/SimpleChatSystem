import websocket
import threading

class WebsocketAPIWrapper:
    wsapp = None

    #on_open = None
    on_message = None
    on_close = None 
    on_error = None

    def __init__(
            self,
            ws_endpoint, 
            on_open=None, 
            on_message=None, 
            on_close=None, 
            on_error=None
    ):
        self.on_open=on_open
        self.on_message=on_message
        self.on_close=on_close
        self.on_error=on_error

        self.wsapp = websocket.WebSocketApp(
            ws_endpoint,
            on_open=self._on_open,
            on_message=self._on_message,
            on_close=self._on_close,
            on_error=self._on_error
        )
    
    def _on_open(self, wsapp):
        self.on_open()
    
    def _on_message(self, wsapp, msg):
        self.on_message(msg)

    def _on_close(self, wsapp, status_code, msg):
        self.on_close()
    
    def _on_error(self, wsapp, ec):
        self.on_error()
    
    def SetUsernameHeader(self, username):
        self.wsapp.header = {"Username": username}

    def Send(self, data):
        self.wsapp.send(data)
    
    def Run(self):
        t = threading.Thread(self.wsapp.run_forever())
        t.daemon = True
        t.start()


