from ChatUI import ChatUI

def main():
    exchange_ui = ChatUI(ws_endpoint="ws://localhost:9002")
    exchange_ui.Start()



if(__name__ == "__main__"):
    main()