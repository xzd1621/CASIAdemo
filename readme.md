5.27
Author只有管理员登录之后才能GET，POST
Publisher未登录的用户只能GET，已登录的管理员或普通用户都可以GET，POST


5.28
增加会话机制，会话时间设置为15S，可以在MyAuth.py的request.session.set_expiry()函数中进行修改。管理员登录之后，在15秒内再次发起GET，POST等请求时无需登录。登录时，若用户名密码输入错误或者未输入，则提示“请输入正确的用户名密码"，若用户名密码正确但不是管理员，则提示”您不是管理员，无法访问"。对于Publisher，发起POST请求时，若未输入正确的用户名密码，则提示"输入正确的用户名密码之后才能POST"。对于Author和Publisher均设有限制，访问太频繁，就返回"请求超过限速"。
