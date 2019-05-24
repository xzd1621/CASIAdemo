from django.contrib import admin

# Register your models here.
from .models import Publisher, Author, Book


@admin.register(Publisher)
class PublisherAdmin(admin.ModelAdmin):
    list_filter = ('name',)
    search_fields = ('name',)


@admin.register(Author)
class AuthorAdmin(admin.ModelAdmin):
    search_fields = ('name', )
    ordering = ('name',)


@admin.register(Book)
class BookAdmin(admin.ModelAdmin):
    search_fields = ('title', 'author', 'publisher')
    list_filter = ('author', 'publisher')
