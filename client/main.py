from ChatUI import ChatUI

def main():
    exchange_ui = ChatUI(rest_api_endpoint="https://127.0.0.1:8080", ws_endpoint="ws://localhost:9002")
    exchange_ui.Start()



if(__name__ == "__main__"):
    main()