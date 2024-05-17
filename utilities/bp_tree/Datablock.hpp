class DataBlock {
private:
  struct DataNode {

    DataNode() { prev_ = next_ = -1; }

    int prev_, next_;
    Data data_;
  };

  DataNode *head_, *tail_;
  std::fstream file_;
  std::string filename_;

  template <class Type>
  inline void read(Type *data, int position,
                   std::ios_base::seekdir mode = std::ios::beg) {
    file_.seekg(position, mode);
    file_.read(reinterpret_cast<char *>(data), sizeof(data));
    return;
  }

  template <class Type>
  inline void write(Type *data, int position,
                    std::ios_base::seekdir mode = std::ios::beg) {
    file_.seekp(position, mode);
    file_.write(reinterpret_cast<char *>(data), sizeof(data));
    return;
  }

public:
  DataBlock(std::string filename = "datablock") {
    head_ = new DataNode;
    tail_ = new DataNode;
  }

  DataBlock(int position, std::string filename = "datablock") {
    filename_ = filename;
    file_.open(filename_, std::ios::in | std::ios::out | std::ios::binary);
    head_ = new DataNode;
    read(head_, position);
  }
};