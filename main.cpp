#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <memory>


// Иерархия Исключений

// Базовый класс ошибок
class FleetError : public std::exception {
  public:
    FleetError(const std::string& message) : message_(message) {}
    const char* what() const noexcept override { return message_.c_str(); }
  private:
    std::string message_;
};

// Ошибка техосмотра
class InspectionRejected : public FleetError {
  public:
    InspectionRejected() : FleetError("inspection rejected") {}
};

// Ошибка ввода данных
class ParseError : public FleetError {
  public:
    ParseError(const std::string& message) : FleetError(message) {}
};

// Базовые классы

// Предмет, использующий энергию
class EnergyConsumer {
  public:
    virtual double DailyEnergy() const = 0;
    virtual ~EnergyConsumer() {}
};

// Предмет
class InventoryItem {
  public:
    virtual std::string InventoryId() const = 0;
    virtual std::string Name() const = 0;
    virtual void Describe() const = 0;
    virtual bool Usable() const { return false; }
    virtual const EnergyConsumer* AsConsumer() const { return nullptr; }
    virtual ~InventoryItem() {}
};

// Транспорт
class Transport : public InventoryItem {
  public:
    Transport(const std::string& id, const std::string& name, const int& score)
      : score_(score), id_(id), name_(name) {
      if (score_ > 10 || score_ < 1) {
        throw ParseError("wrong score");
      }
      if (id_.empty()) {
        throw ParseError("empty id");
      }
    }
    bool Usable() const override {
      if (score_ >= 6) { 
        return true;
      } else {
        return false;
      }
    }
    std::string InventoryId() const override { return id_; }
    std::string Name() const override { return name_; }
    int Score() const { return score_; }
  private:
    int score_;
    std::string id_;
    std::string name_;
};

// Обычные классы

// Велосипед
class Bike : public Transport {
  public:
    Bike(const std::string& id, const std::string& name, const int& score)
      : Transport(id, name, score) {}
    void Describe() const override {
      std::cout << "Bike " << "Name: " << Name() << " ID: " << InventoryId() << " Score: " << Score() << "." << "\n";
    }
};

// Шлем
class Helmet: public InventoryItem {
  public:
    Helmet(const std::string& id, const std::string& name)
      : id_(id), name_(name) {
      if (id_.empty()) {
        throw ParseError("empty id");
      }
    }
    std::string InventoryId() const override { return id_; }
    std::string Name() const override {return name_; }
    void Describe() const override {
      std::cout << "Helmet " << "Name: " << name_ << " ID: " << id_<< "." << "\n";
    }

  private:
    std::string id_;
    std::string name_;
};

// Док-станция
class DockStation: public InventoryItem, public EnergyConsumer {
  public:
    DockStation(const std::string& id, const std::string name, const double& energy)
      : id_(id), name_(name), energy_(energy) {
      if (id_.empty()) {
        throw ParseError("empty id");
      }
      if (energy_ < 0) {
        throw ParseError("wrong energy");
      }
    }
    std::string InventoryId() const override { return id_; }
    std::string Name() const override { return name_; }
    void Describe() const override {
      std::cout << "DockStation " << "Name: " << name_ << " ID: " << id_<< "." << "\n";
    }
    double DailyEnergy() const override { return energy_; }
    const EnergyConsumer* AsConsumer() const override { return this; }
  private:
    std::string id_;
    std::string name_;
    double energy_;
};

// Электрический скутер
class ElectricScooter : public Transport, public EnergyConsumer {
  public:
    ElectricScooter(const std::string& id, const std::string& name, const double& energy, const int& score)
      : Transport(id, name, score), energy_(energy) {
      if (energy_ < 0) {
        throw ParseError("wrong energy");
      }
    }
    void Describe() const override {
      std::cout << "ElectricScooter " << "Name: " << Name() << " ID: " << InventoryId() << " Energy: " << energy_ << " Score: " << Score() << "." << "\n";
    }
    double DailyEnergy() const override { return energy_; }
    const EnergyConsumer* AsConsumer() const override { return this; }
  private:
    double energy_;
};

// Электрический велосипед
class EBike : public Transport, public EnergyConsumer {
  public:
    EBike(const std::string& id, const std::string& name, const double& energy, const int& score) 
      : Transport(id, name, score), energy_(energy) {
      if (energy_ < 0) {
        throw ParseError("wrong energy");
      }
    }
    void Describe() const override {
      std::cout << "EBike " << "Name: " << Name() << " ID: " << InventoryId() << " Energy: " << energy_ << " Score: " << Score() << "." << "\n";
    }
    double DailyEnergy() const override { return energy_; }
    const EnergyConsumer* AsConsumer() const override { return this; }
  private:
    double energy_;
};

// Скейтборд
class Skateboard : public Transport {
  public:
    Skateboard(const std::string& id, const std::string& name, const int& score, const int& length)
      : Transport(id, name, score), length_(length) {
      if (length_ <= 0) {
        throw ParseError("wrong length");
      }
    }
    bool Usable() const override {
      return Score() >= 8;
    }
    void Describe() const override {
      std::cout << "Skateboard " << "Name: " << Name() << " ID: " << InventoryId() << " Length: " << length_ << " Score: " << Score() << "." << "\n";
    }
  private:
    int length_;
};

// Сервис Центр
class ServiceCenter {
  public:  
    void Inspect(const InventoryItem& item) const {
      std::string s = item.Name();
      for (int i = 0; i < s.size(); i++) {
        s[i] = std::tolower(s[i]);
      }
      for (int i = 0; i + 6 <= s.size(); i++) {
        if (s.substr(i, 6) == "broken") {
          throw InspectionRejected();
        }
      }
    }
};

// Журнал
template <typename T, int Cap>
class Registry {
  public:
    void Add(const T& value) {
      if (size_ == Cap) {
        throw ParseError("registry is full");
      }
      data_[size_] = value;
      size_++;
    }
    int Size() const { return size_; }
    const T& operator[](int i) const {
      if (i < 0 || i >= size_) {
        throw ParseError("wrong index");
      }
      return data_[i];
    }
  private:
    T data_[Cap];
    int size_ = 0;
};

template <typename T, int Cap>
std::ostream& operator<<(std::ostream& out, const Registry<T, Cap>& registry) {
  for (int i = 0; i < registry.Size(); i++) {
    out << registry[i] << "\n";
  }
  return out;
}

// Операторы
std::ostream& operator<<(std::ostream& out, const InventoryItem& item) {
  out << item.InventoryId() << " " << item.Name();
  return out;
}

bool operator<(const InventoryItem& a, const InventoryItem& b) {
  return a.InventoryId() < b.InventoryId();
}

// Парк
class Park {
  public:
    void AddThing(std::unique_ptr<InventoryItem> item) {
      items_.push_back(std::move(item));
    }
    void AddTransport(std::unique_ptr<Transport> item) {
      try {
        center_.Inspect(*item);
      } catch (const InspectionRejected&) {
        rejected_.Add(item->Name());
        throw;
      }
      items_.push_back(std::move(item));
    }

    const Registry<std::string, 100>& Rejected() const { return rejected_; }

    void Remove(const std::string& id) {
      for (int i = 0; i < items_.size(); i++) {
        if (items_[i]->InventoryId() == id) {
          items_.erase(items_.begin() + i);
          return;
        }
      }
      throw ParseError("unknown id");
    }

    double TotalEnergy() const {
      double total = 0;
      for (const auto& item : items_) {
        const EnergyConsumer* consumer = item->AsConsumer();
        if (consumer != nullptr) {
          total += consumer->DailyEnergy();
        }
      }
      return total;
    }

    void PrintNovice() const {
      for (const auto& item : items_) {
        if (item->Usable()) {
          std::cout << *item << "\n";
        }
      }
    }

    void PrintAll() const {
      std::vector<const InventoryItem*> sorted;
      for (const auto& item : items_) {
        sorted.push_back(item.get());
      }
      std::sort(sorted.begin(), sorted.end(), [](const InventoryItem* a, const InventoryItem* b) { return *a < *b; });
      for (const InventoryItem* item : sorted) {
        std::cout << *item << "\n";
      }
    }

    void Focus(const std::string& id) {
      for (const auto& item : items_) {
        if (item->InventoryId() == id) {
          focus_ = item;
          return;
        }
      }
      throw ParseError("unknown id");
    }

    void ShowFocus() const {
      std::shared_ptr<InventoryItem> item = focus_.lock();
      if (item) {
        std::cout << *item << "\n";
      } else {
        std::cout << "removed" << "\n";
      }
    }

    double MaxEnergy() const {
      double result = 0;
      for (const auto& item: items_) {
        const EnergyConsumer* consumer = item->AsConsumer();
        if (consumer != nullptr) {
          result = std::max(consumer->DailyEnergy(), result);
        }
      }
      return result;
    }

  private:
    std::vector<std::shared_ptr<InventoryItem>> items_;
    std::weak_ptr<InventoryItem> focus_;
    ServiceCenter center_;
    Registry<std::string, 100> rejected_;
};

int main() {
  Park park;
  std::string s;
  while (std::cin >> s && s != "quit") {
    try {
      if (s == "add") {
        std::string type, id, name;
        std::cin >> type >> id >> name;
        if (type == "scooter") {
          double energy;
          int score;
          std::cin >> energy >> score;
          park.AddTransport(std::make_unique<ElectricScooter>(id, name, energy, score));
        } else if (type == "bike") {
          int score;
          std::cin >> score;
          park.AddTransport(std::make_unique<Bike>(id, name, score));
        } else if (type == "ebike") {
          double energy;
          int score;
          std::cin >> energy >> score;
          park.AddTransport(std::make_unique<EBike>(id, name, energy, score));
        } else if (type == "skate") {
          int score;
          int length;
          std::cin >> score >> length;
          park.AddTransport(std::make_unique<Skateboard>(id, name, score, length));
        } else if (type == "helmet") {
          park.AddThing(std::make_unique<Helmet>(id, name));
        } else if (type == "dock") {
          double energy;
          std::cin >> energy;
          park.AddThing(std::make_unique<DockStation>(id, name, energy));
        }
      } else if (s == "energy") {
        std::cout << std::fixed << std::setprecision(1) << park.TotalEnergy() << "\n";
      } else if (s == "novice") {
        park.PrintNovice();
      } else if (s == "list") {
        park.PrintAll();
      } else if (s == "focus") {
        std::string id;
        std::cin >> id;
        park.Focus(id);
      } else if (s == "show_focus") {
        park.ShowFocus();
      } else if (s == "remove") {
        std::string id;
        std::cin >> id;
        park.Remove(id);
      } else if (s == "max_energy") {
        std::cout << std::fixed << std::setprecision(1) << park.MaxEnergy() << "\n";
      } else if (s == "rejected") {
        std::cout << park.Rejected();
      }
    } catch (const FleetError& e) {
      std::cout << "error: " << e.what() << "\n";
    }
  }
}
