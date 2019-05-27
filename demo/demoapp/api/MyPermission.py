from rest_framework.permissions import BasePermission


class AuthorPermission(BasePermission):

    def has_permission(self, request, view):
        if request.user and request.user.user_type == 2:
            return True
        else:
            return False


class PublisherPermission(BasePermission):

    def has_permission(self, request, view):
        if request.method == 'GET':
            return True
        if request.user:
            return True
        else:
            return False
