#include <iostream>
#include <sstream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

// 替换为你自己的 API Key
const std::string API_KEY = "你的API_Key";
const std::string API_URL = "https://api.deepseek.com/chat/completions";

// 回调函数，用于接收响应数据
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "用法: " << argv[0] << " <你的消息>" << std::endl;
        return 1;
    }

    std::string userMessage = argv[1];

    // 构造JSON请求体
    Json::Value root;
    root["model"] = "deepseek-chat";
    root["stream"] = false;

    Json::Value messages(Json::arrayValue);
    Json::Value sysMsg;
    sysMsg["role"] = "system";
    sysMsg["content"] = "You are a helpful assistant.";
    messages.append(sysMsg);

    Json::Value userMsg;
    userMsg["role"] = "user";
    userMsg["content"] = userMessage;
    messages.append(userMsg);

    root["messages"] = messages;

    Json::StreamWriterBuilder writer;
    std::string requestBody = Json::writeString(writer, root);

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "无法初始化 curl！" << std::endl;
        return 1;
    }

    std::string responseStr;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "请求失败: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_cleanup(curl);

    // 解析JSON响应
    Json::CharReaderBuilder reader;
    Json::Value jsonResponse;
    std::string errs;
    std::istringstream s(responseStr);
    if (!Json::parseFromStream(reader, s, &jsonResponse, &errs)) {
        std::cerr << "解析响应失败: " << errs << std::endl;
        return 1;
    }

    // 输出回复内容
    try {
        std::string reply = jsonResponse["choices"][0]["message"]["content"].asString();
        std::cout << "🤖 DeepSeek 回复：" << reply << std::endl;
    } catch (...) {
        std::cerr << "无法提取回复内容，请检查响应格式。" << std::endl;
        std::cerr << "原始响应：" << responseStr << std::endl;
    }

    return 0;
}
