from Protobuf import kick_user_pb2, connected_users_pb2
from google.protobuf.json_format import MessageToDict
import requests

class RestAPIWrapper:
    api_endpoint = None

    def __init__(self, api_endpoint):
        self.api_endpoint = api_endpoint.strip()
        if(self.api_endpoint[-1] == '/'):
            self.api_endpoint = self.api_endpoint[: -1]
    
    def _CheckStatusCode(self, status_code):
        if( (status_code < 200) or (status_code > 299) ):
            raise Exception("Connection error, Status code: " + str(status_code))

    def _Request_POST(self, service_path, data=None):
        service_path = service_path.strip()
        if(service_path[0] != '/'):
            service_path = '/' + service_path

        resp = requests.post(self.api_endpoint + service_path, data=data, verify=self.path_to_pub_key)

        self._CheckStatusCode(resp.status_code)

        return resp.text
    
    def KickUser(self, username):
        kick_user_msg = kick_user_pb2.KickUser()
        kick_user_msg.username = username

        return self._Request_POST("/kick_user", data=kick_user_msg.SerializeToString())
    
    def GetUsers(self):
        connected_users_msg = connected_users_pb2.ConnectedUsers()
        connected_users_msg.ParseFromString(self._Request_POST("/get_users"))

        return MessageToDict(connected_users_msg, descriptor_pool=connected_users_pb2)