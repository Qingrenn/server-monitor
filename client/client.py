import requests
import os
import os.path as osp
import uuid
import time

host = 'http://172.20.111.236'
port = '8888'
dest = 'state.json'
url = osp.join(host+":"+port, dest)

DEVICE_NAME = 'TestMachine'
MAC = uuid.UUID(int=uuid.getnode()).hex[-12:]

def localquery():
    netcard_names = ['ppp0', 'enp34s0']
    body = {'mac': MAC, 'device_name': DEVICE_NAME}
    for name in netcard_names:
        ip = os.popen(f'ip addr show {name}').read().split("inet ")[1].split(" ")[0].split("/")[0]
        body[name] = ip
    try:
        res = requests.post(url=url,data=body)
        print(f"{res}: {body}")
    except requests.exceptions.ConnectionError as e:
        print(f'Connection Failed ... \n {e}')

if __name__ == '__main__':
    
    while(True):
        localquery()
        time.sleep(300)
    