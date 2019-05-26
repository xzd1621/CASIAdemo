
from rest_framework.authentication import BaseAuthentication

from demoapp.models import MyUser


class AuthorAuth(BaseAuthentication):
    def authenticate(self, request):
        if request.method in ['POST', 'GET', 'PUT', 'DELETE']:
            username = request.GET.get('username')
            password=request.GET.get('password')
            print(username, password)
            try:
                user = MyUser.objects.filter(username=username,password=password).first()
                print(user)
                return user.username, None
            except Exception as e:
                return None, None
        else:
            return None, None


class PublisherAuth(BaseAuthentication):
    def authenticate(self, request):
        if request.method in ['GET', 'PUT', 'DELETE']:
            username = request.GET.get('username')
            password=request.GET.get('password')
            print(username, password)
            try:
                user = MyUser.objects.filter(username=username,password=password).first()
                print(user)
                return user, None
            except Exception as e:
                return None, None
        else:
            return None, None
