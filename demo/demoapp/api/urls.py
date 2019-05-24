from django.urls import path
from . import views


app_name = 'demoapp'

urlpatterns = [
    path('publisher/',views.PublisherListView.as_view(),name='publisher_list'),
    path('publisher/<pk>/',views.PublisherDetailView.as_view(),name='publisher_detail'),
    path('author/', views.AuthorListView.as_view(), name='author_list'),
    path('author/<pk>/', views.AuthorDetailView.as_view(), name='author_detail'),
    path('book/', views.BookListView.as_view(), name='book_list'),
    path('book/<pk>/', views.BookDetailView.as_view(), name='book_detail'),

]