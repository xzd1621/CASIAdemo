from rest_framework.permissions import BasePermission


class AuthorPermission(BasePermission):

    def has_permission(self, request, view):
        if request.user:
            return True
        else:
            return False


class PublisherPermission(BasePermission):

    def has_permission(self, request, view):
        if request.user:
            return True
        else:
            return False
