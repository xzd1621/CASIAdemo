import json

from rest_framework.authentication import BaseAuthentication

from demoapp.models import MyUser


"""
只有管理员登录之后才能GET，POST等，
其他类别用户即便登录也无法查看信息
登录有会话机制，维持一段时间
"""
class AuthorAuth(BaseAuthentication):
    def authenticate(self, request):
        request.session.set_expiry(10)
        if request.method in ['GET', 'POST', 'PUT', 'DELETE']:
            if 'is_login' not in request.session.keys() or not request.session['is_login']:
                username = request.GET.get('username')
                password=request.GET.get('password')
                request.session['username'] = username
                print(username, password)
                try:
                    user = MyUser.objects.filter(username=username,password=password).first()
                    request.session['is_login'] = True
                    print(user)
                    return user, None
                except Exception as e:
                    return None, None
            else:
                if 'username' not in request.session.keys():
                    return None, None
                else:
                    user = MyUser.objects.filter(username=request.session['username'], ).first()
                    if user is not None:
                        return user, None
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
        request.session.set_expiry(10)
        if request.method in ['POST', 'PUT', 'DELETE']:
            if 'is_login' not in request.session.keys() or not request.session['is_login']:
                username = request.GET.get('username')
                password=request.GET.get('password')
                request.session['username'] = username
                print(username, password)
                try:
                    user = MyUser.objects.filter(username=username,password=password).first()
                    request.session['is_login'] = True
                    print(user)
                    return user, None
                except Exception as e:
                    return None, None
            else:
                if 'username' not in request.session.keys():
                    return None, None
                else:
                    user = MyUser.objects.filter(username=request.session['username'], ).first()
                    if user is not None:
                        return user, None
                    else:
                        return None, None
        else:
            return None, None
