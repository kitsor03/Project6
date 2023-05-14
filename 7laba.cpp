// Підключаємо потрібні бібліотеки
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Функція зворотного виклику для запису відповіді в рядок
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb); // Додаємо дані до рядка
    return size * nmemb; // Повертаємо кількість оброблених байт
}

// Функція для отримання інформації про IP
json getIpInfo(const std::string& ip) {
    CURL* curl; // Об'єкт CURL
    CURLcode res; // Код відповіді
    std::string readBuffer; // Рядок для зберігання відповіді

    curl_global_init(CURL_GLOBAL_DEFAULT); // Ініціалізація CURL
    curl = curl_easy_init(); // Створення об'єкта CURL
    if (curl) {
        std::string url = "https://worldtimeapi.org/api/ip/" + ip + ".json"; // Формуємо URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Встановлюємо URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Встановлюємо функцію зворотного виклику для запису відповіді
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // Встановлюємо рядок, в який буде записуватися відповідь
        res = curl_easy_perform(curl); // Виконуємо запит
        curl_easy_cleanup(curl); // Звільняємо ресурси CURL
    }
    curl_global_cleanup(); // Звільняємо глобальні ресурси CURL
    return json::parse(readBuffer); // Парсимо JSON і повертаємо його
}

int main() {
    std::string ip;
    std::cout << "Enter IP address: "; // Запитуємо IP-адресу від користувача
    std::cin >> ip;

    json ipInfo = getIpInfo(ip); // Отримуємо інформацію про IP

    // Виводимо інформацію на консоль
    std::cout << "Timezone: " << ipInfo["timezone"] << std::endl;
    std::cout << "DateTime: " << ipInfo["datetime"] << std::endl;
    std::cout << "Day of the year: " << ipInfo["day_of_year"] << std::endl;

    return 0; // Завершуємо програму
}