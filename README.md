    # Dice-of-COC Version 1.3.5

COC跑团用骰子，开源，使用VC++2017编写

用法：.o(或.rd)<space>骰子类型<space>原因(可选) (注意有一个.)
 
暗骰请用.h代替.o(或.rd)

软件带有COC6,COC7的人物作成与DND英雄作成,命令为.o(或.rd) COC7/COC6/DND

骰子类型比如1d100，d的大写小写均可，如果只有一个骰子可以省略1，支持骰子使用“+”进行叠加和"-"进行相减，也支持直接加上或减去数字，如.o d10-d6+2, 不输入骰子类型则默认为D100

暂不支持自定义掷骰，这个功能会在接下来的版本中实现

支持多轮掷骰,使用符号#,前面加上数字或骰子代表轮数，后面加上骰子类型，如.o 5#1d3+3就是骰5次1d3+3

源代码可以修改编译后自用，但不能发布或者商业使用

有问题请联系QQ1840686745(或者直接提issue) 希望大家能够支持，谢谢

PS:未安装VC++2017运行库程序将报错(-103),微软官方下载地址：64位:https://aka.ms/vs/15/release/VC_redist.x64.exe 
32位: https://aka.ms/vs/15/release/VC_redist.x86.exe

PPS:最近有人反馈在群中收不到骰子的问题，经检查应该是由于小号是新建的，并且短时间发送了大量信息，所以被腾讯误认为是广告之类的给拦截了，此问题暂时无解，可以换老一点了小号或者是将你的机器人小号闲置一段时间便可以恢复正常
