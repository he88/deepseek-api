# deepseek-api
一个用 C++ 编写的命令行工具，通过 DeepSeek API 发送消息并接收回复。你可以像使用 ChatGPT 一样，在终端里与 DeepSeek 聊天。        

## 🔑 设置你的 API Key
在 `main.cpp` 文件顶部找到以下这一行：
```cpp
const std::string API_KEY = "你的API Key";
```

将其中的 "你的DeepSeek_API_Key" 替换成你在 DeepSeek 官网 获取到的真实 API Key，例如：
```cpp
const std::string API_KEY = "sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
```
当然，这种做法并不值得推荐，建议还是把API key放在本地文件中调用，以防止泄漏。但是我懒。  

## 🔧 编译方法

确保你已经安装了以下依赖：
```bash
sudo apt-get install libcurl4-openssl-dev libjsoncpp-dev
```

然后使用以下命令编译：
```bash
g++ -o send_message main.cpp -lcurl -ljsoncpp
```
  
## 🚀 使用方法

执行可执行文件并传入你想要发送的消息：
```bash
./send_message "你好，介绍一下北京邮电大学"
```
它会把消息发送到 DeepSeek，并在终端中打印回复。
  
## 🛠️ TODO（下次一定）
- 从本地文件读取 API Key，避免硬编码

- 支持持续对话上下文

