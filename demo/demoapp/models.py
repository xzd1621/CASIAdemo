from django.db import models

# Create your models here.


class MyUser(models.Model):
    username = models.CharField(max_length=30, unique=True)
    password = models.CharField(max_length=30)
    user_type = models.IntegerField(
        choices=((1, 'ordinary'), (2, 'vip')),
        default=1
    )


class Publisher(models.Model):
    name = models.CharField(max_length=30)
    address = models.CharField(max_length=50)

    def __str__(self):
        return self.name


class Author(models.Model):
    name = models.CharField(max_length=30)
    email = models.EmailField()

    def __str__(self):
        return self.name


class Book(models.Model):
    title = models.CharField(max_length=50)
    author = models.ManyToManyField(Author)
    publisher = models.ForeignKey(Publisher, on_delete=models.CASCADE)

    class Meta:
        ordering = ('-title', )

    def __str__(self):
        return self.title
