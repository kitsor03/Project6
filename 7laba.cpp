// ϳ�������� ������ ��������
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ������� ���������� ������� ��� ������ ������ � �����
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb); // ������ ��� �� �����
    return size * nmemb; // ��������� ������� ���������� ����
}

// ������� ��� ��������� ���������� ��� IP
json getIpInfo(const std::string& ip) {
    CURL* curl; // ��'��� CURL
    CURLcode res; // ��� ������
    std::string readBuffer; // ����� ��� ��������� ������

    curl_global_init(CURL_GLOBAL_DEFAULT); // ����������� CURL
    curl = curl_easy_init(); // ��������� ��'���� CURL
    if (curl) {
        std::string url = "https://worldtimeapi.org/api/ip/" + ip + ".json"; // ������� URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // ������������ URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // ������������ ������� ���������� ������� ��� ������ ������
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // ������������ �����, � ���� ���� ������������ �������
        res = curl_easy_perform(curl); // �������� �����
        curl_easy_cleanup(curl); // ��������� ������� CURL
    }
    curl_global_cleanup(); // ��������� �������� ������� CURL
    return json::parse(readBuffer); // ������� JSON � ��������� ����
}

int main() {
    std::string ip;
    std::cout << "Enter IP address: "; // �������� IP-������ �� �����������
    std::cin >> ip;

    json ipInfo = getIpInfo(ip); // �������� ���������� ��� IP

    // �������� ���������� �� �������
    std::cout << "Timezone: " << ipInfo["timezone"] << std::endl;
    std::cout << "DateTime: " << ipInfo["datetime"] << std::endl;
    std::cout << "Day of the year: " << ipInfo["day_of_year"] << std::endl;

    return 0; // ��������� ��������
}