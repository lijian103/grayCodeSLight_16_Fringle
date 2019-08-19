def my_max(x, y) :
    '''
    获取两个数值之间较大数的函数。
    my_max(x, y)
        返回x、y两个参数之间较大的那个
    '''
    # 定义一个变量z，该变量等于x、y中较大的值
    z = x if x > y else y
    # 返回变量z的值
    return z
def test ():
    age = 20
    # 直接访问age局部变量
    print(age) # 输出20
    # 访问函数局部范围的“变量数组”
    print(locals()) # {'age': 20}
    # 通过函数局部范围的“变量数组”访问age变量
    print(locals()['age']) # 20
    # 通过locals函数局部范围的“变量数组”改变age变量的值
    locals()['age'] = 12
    # 再次访问age变量的值
    print('xxx', age) # 依然输出20
    # 通过globals函数修改x全局变量
    globals()['x'] = 19


