from rest_framework.throttling import UserRateThrottle, AnonRateThrottle


class AuthorThrottle(UserRateThrottle):
    scope = 'author'

    def get_cache_key(self, request, view):
        return self.get_ident(request)


class BookThrottle(AnonRateThrottle):
    scope = 'book'

    def get_cache_key(self, request, view):
        return self.get_ident(request)
