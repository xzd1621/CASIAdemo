from django.views.decorators.csrf import csrf_exempt
from rest_framework import generics, mixins, exceptions
from rest_framework.authentication import BasicAuthentication
from rest_framework.permissions import IsAuthenticated, AllowAny
from rest_framework.response import Response

from .MyPermission import AuthorPermission, PublisherPermission
from .MyAuth import PublisherAuth, AuthorAuth

from ..models import Publisher, Author, Book
from .serializers import PublisherSerializer, AuthorSerializer, BookSerializer
from .utils.mythrottle import AuthorThrottle, BookThrottle, PublisherThrottle


class PublisherListView(mixins.ListModelMixin, mixins.CreateModelMixin,  generics.GenericAPIView):
    authentication_classes = [PublisherAuth, ]
    permission_classes = [PublisherPermission, ]
    throttle_classes = [PublisherThrottle, ]
    queryset = Publisher.objects.all()
    serializer_class = PublisherSerializer

    def get(self, request, *args, **kwargs):
        return self.list(request, *args, **kwargs)

    def post(self, request, *args, **kwargs):
        return self.create(request, *args, **kwargs)

    def permission_denied(self, request, message=None):
        if request.authenticators and request.user is None:
            raise exceptions.NotAuthenticated(detail='输入正确的用户名密码之后才能POST')
        raise exceptions.PermissionDenied(detail=message)

    # def throttled(self, request, wait):
    #     '''
    #     访问次数被限制时，定制错误信息
    #     :param request:
    #     :param wait:
    #     :return:
    #     '''
    #
    #     class Throttled(exceptions.Throttled):
    #         default_detail = '请求被限制'
    #         extra_detail_singular = '请 {wait} 秒后再重试'
    #         extra_detail_plural = '请{wait}秒后再试'
    #
    #     return Throttled(wait)


class PublisherDetailView(mixins.RetrieveModelMixin,
                          mixins.UpdateModelMixin,
                          mixins.DestroyModelMixin,
                          generics.GenericAPIView):
    authentication_classes = [PublisherAuth, ]
    permission_classes = [PublisherPermission, ]
    throttle_classes = [PublisherThrottle, ]
    queryset = Publisher.objects.all()
    serializer_class = PublisherSerializer

    def get(self, request, *args, **kwargs):
        return self.retrieve(request, *args, **kwargs)

    def put(self, request, *args, **kwargs):
        return self.update(request, *args, **kwargs)

    def delete(self, request, *args, **kwargs):
        return self.destroy(request, *args, **kwargs)

    # def throttled(self, request, wait):
    #     '''
    #     访问次数被限制时，定制错误信息
    #     :param request:
    #     :param wait:
    #     :return:
    #     '''
    #
    #     class Throttled(exceptions.Throttled):
    #         default_detail = '请求被限制'
    #         extra_detail_singular = '请 {wait} 秒后再重试'
    #         extra_detail_plural = '请{wait}秒后再试'
    #
    #     return Throttled(wait)


class AuthorListView(mixins.ListModelMixin, mixins.CreateModelMixin,  generics.GenericAPIView):
    authentication_classes = [AuthorAuth, ]
    permission_classes = [AuthorPermission, ]
    throttle_classes = [AuthorThrottle, ]
    message = '无权访问'
    queryset = Author.objects.all()
    serializer_class = AuthorSerializer

    def get(self, request, *args, **kwargs):
        return self.list(request, *args, **kwargs)

    def post(self, request, *args, **kwargs):
        return self.create(request, *args, **kwargs)

    def permission_denied(self, request, message=None):
        if request.authenticators:
            if request.user is None:
                raise exceptions.NotAuthenticated(detail='请输入正确的用户名密码')
            elif request.user.user_type == 1:
                raise exceptions.NotAuthenticated(detail='您不是管理员，无法访问')
            raise exceptions.NotAuthenticated(detail='用户名或密码错误')
        raise exceptions.PermissionDenied(detail=message)


class AuthorDetailView(mixins.RetrieveModelMixin,
                       mixins.UpdateModelMixin,
                       mixins.DestroyModelMixin,
                       generics.GenericAPIView):
    authentication_classes = [AuthorAuth, ]
    permission_classes = [AuthorPermission, ]
    throttle_classes = [AuthorThrottle, ]

    queryset = Author.objects.all()
    serializer_class = AuthorSerializer

    def get(self, request, *args, **kwargs):
        return self.retrieve(request, *args, **kwargs)

    def put(self, request, *args, **kwargs):
        return self.update(request, *args, **kwargs)

    def delete(self, request, *args, **kwargs):
        return self.destroy(request, *args, **kwargs)

    def permission_denied(self, request, message=None):
        if request.authenticators:
            if request.user is None:
                raise exceptions.NotAuthenticated(detail='请输入正确的用户名密码')
            elif request.user.user_type == 1:
                raise exceptions.NotAuthenticated(detail='您不是管理员，无法访问')
            raise exceptions.NotAuthenticated(detail='用户名或密码错误')
        raise exceptions.PermissionDenied(detail=message)


class BookListView(mixins.ListModelMixin, mixins.CreateModelMixin,  generics.GenericAPIView):
    authentication_classes = (BasicAuthentication,)
    permission_classes = (AllowAny,)
    queryset = Book.objects.all()
    serializer_class = BookSerializer
    throttle_classes = [BookThrottle, ]

    def get(self, request, *args, **kwargs):
        return self.list(request, *args, **kwargs)

    def post(self, request, *args, **kwargs):
        return self.create(request, *args, **kwargs)


class BookDetailView(mixins.RetrieveModelMixin,
                     mixins.UpdateModelMixin,
                     mixins.DestroyModelMixin,
                     generics.GenericAPIView):
    authentication_classes = (BasicAuthentication,)
    permission_classes = (AllowAny,)
    queryset = Book.objects.all()
    serializer_class = BookSerializer
    throttle_classes = [BookThrottle, ]

    def get(self, request, *args, **kwargs):
        return self.retrieve(request, *args, **kwargs)

    def put(self, request, *args, **kwargs):
        return self.update(request, *args, **kwargs)

    def delete(self, request, *args, **kwargs):
        return self.destroy(request, *args, **kwargs)
