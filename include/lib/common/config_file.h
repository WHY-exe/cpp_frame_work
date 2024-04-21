#pragma once
#include <iostream>
#include <string>
#include <vector>
/**
 * @brief .ini file parsing, won''t updating the file when the instance is
 * destroyed not thread-safe, watch for race condition
 */
namespace util {
class Config {
  struct Info {
    std::string key;
    std::string value;
  };

  class Section {
    friend class Config;
    friend std::ostream &operator<<(std::ostream &os, const Section &section);

  private:
    std::string name_;
    mutable std::vector<Info> config_;

  public:
    Section() noexcept = default;
    ~Section() noexcept = default;

  private:
    /**
     * @brief Construct a new Config Section object
     *        by calling the Init method
     * @param name
     */
    explicit Section(std::string &&name) noexcept;
    /**
     * @brief initialize the name of the section
     * @param [in] name
     */
    void Init(std::string &&name) noexcept;
    /**
     * @brief add condifg name and value to the vector
     * @param key config key
     * @param value config value
     */
    void AddConf(std::string &&key, std::string &&value);

  public:
    /**
     * @brief tell wether the name of the section is empty
     * @return wether the name of the section is empty
     */
    bool IsInit() const noexcept;
    /**
     * @brief writing to config stored in memory
     * @param [in] key config name
     * @param [in] value config value
     */
    void WriteConf(const std::string &key, const std::string &value);
    /**
     * @brief reading from config stored in memory
     * @param [in] key config name
     * @return config value
     */
    const std::string &ReadConf(const std::string &key) const;
    /**
     * @brief get the reference of the value
     *        corresponding to specific key
     * @param [in] key the config key
     * @return the reference of the value
     */
    std::string &operator[](const std::string &key);
    const std::string &operator[](const std::string &key) const;
  };
  friend std::ostream &operator<<(std::ostream &os, const Section &section);

private:
  std::string file_path_;
  mutable std::vector<Section> sections_;
  bool save_file_on_destroyed_;

private:
  int InitConfig();
  /**
   * @brief updating the .ini file by writing sections
   *        to the .ini file
   */
  void UpdateFile() const;
  /**
   * @brief clear the Section buffer
   */
  void Clear() noexcept;

public:
  Config() noexcept = default;
  ~Config() noexcept;
  /**
   * @brief reading the file and fill the
   *        section buffer
   * @param [in] file_path the ini file path
   */
  bool Init(const std::string &file_path);
  /**
   * @brief Construct a new Config object
   *        by calling the Init method
   * @param [in] file_path
   */
  explicit Config(const std::string &file_path);

  /**
   * @brief get the config section with specific
   *        section name
   * @param [in] section_name the section name
   * @return the corresponding section reference
   */
  Section &GetSection(std::string &&section_name);
  /**
   * @brief sugar of the GetSection method
   *
   * @param section_name
   * @return Section&
   */
  Section &operator[](std::string &&section_name);
  /**
   * @brief update the file with the config section and
   *        clear the section buffer
   */
  void Close();
  /**
   * @brief updating the section buffer by
   *        rereading the ini file
   * @return the number of the current sections
   */
  int UpdateConfig();
};

} // namespace util
