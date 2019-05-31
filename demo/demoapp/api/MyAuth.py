import datetime
import json
import hashlib
import time
from django.http import HttpResponse
from rest_framework.authentication import BaseAuthentication

from demoapp.models import MyUser


"""
只有管理员登录之后才能GET，POST等，
其他类别用户即便登录也无法查看信息
登录有会话机制，维持一段时间
"""
user_message = {}


class AuthorAuth(BaseAuthentication):
    def authenticate(self, request):
        if request.method in ['GET', 'POST', 'PUT', 'DELETE']:
            if request.GET.get('uuid') is None :
                if request.GET.get('username') is not None and request.GET.get('password') is not None:
                    username = request.GET.get('username')
                    password = request.GET.get('password')
                    uuid = str(hashlib.md5(username.encode()).hexdigest())
                    print('生成的uuid：'+uuid)
                    user_message['username'] = username
                    user_message[uuid] = False
                    print(username, password)
                    try:
                        user = MyUser.objects.filter(username=username, password=password).first()
                        user_message[uuid] = True
                        print((datetime.datetime.now() - user.login_time).seconds)
                        MyUser.objects.filter(username=username, password=password).update(login_time=datetime.datetime.now())
                        return user, None
                    except Exception as e:
                        return None, None
                else:
                    return None, None
            else:
                uuid = request.GET.get('uuid')
                print('GET uuid: '+uuid)
                user = MyUser.objects.filter(username=user_message['username'], ).first()
                if (datetime.datetime.now() - user.login_time).seconds > 10:
                    user_message[uuid] = False
                MyUser.objects.filter(username=user_message['username'],).update(login_time=datetime.datetime.now())
                if user_message[uuid]:
                    print('uuid is in seesion.keys')
                    print(user_message[uuid])
                    print(user_message['username'])
                    if user is not None:
                        return user, None
                    else:
                        return None, None
                else:
                    return None, None
        else:
            return None, None


"""
所有类别用户不登录可以查看
登录之后可以POST，GET，PUT
有会话机制
"""
class PublisherAuth(BaseAuthentication):
    def authenticate(self, request):
        # request.session.set_expiry(15)
        if request.method in ['POST', 'PUT', 'DELETE']:
            if request.GET.get('uuid') is None :
                if request.GET.get('username') is not None and request.GET.get('password') is not None:
                    username = request.GET.get('username')
                    password = request.GET.get('password')
                    uuid = str(hashlib.md5(username.encode()).hexdigest())
                    request.session['username'] = username
                    print(username, password)
                    try:
                        user = MyUser.objects.filter(username=username,password=password).first()
                        request.session[uuid] = True
                        print(user)
                        return user, None
                    except Exception as e:
                        return None, None
                else:
                    return None, None
            else:
                uuid = request.GET.get('uuid')
                if uuid in request.session.keys() and request.session[uuid]:
                    user = MyUser.objects.filter(username=request.session['username'], ).first()
                    if user is not None:
                        return user, None
                    else:
                        return None, None
                else:
                    return None, None
        else:
            return None, None
