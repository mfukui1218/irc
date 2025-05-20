#ifndef ICONFIG_HPP
#define ICONFIG_HPP

#include <string>

class IConfig
{
    public:
        virtual ~IConfig() {}

        /// @brief 設定を取得する。設定がない場合はexceptionを投げる。
        /// @param name 設定名
        /// @return 設定値
        virtual std::string getSetting(const std::string &name) const = 0;

        /// @brief 設定を設定する。設定できない項目の場合exceptionを投げる。
        /// @param name 設定名
        /// @param value 設定値
        virtual void setSetting(const std::string &name, const std::string &value) const = 0;
};

#endif // ICONFIG_HPP
