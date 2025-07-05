import kick_user_pb2
from google.protobuf.json_format import MessageToDict

temp = kick_user_pb2.KickUser()
temp.username = "TEMP"

python_dict = MessageToDict(
    temp,
    descriptor_pool=kick_user_pb2)

print(python_dict)