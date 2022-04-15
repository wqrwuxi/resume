class plorg{
  public:
    plorg(const char* new_name,int new_CI = 50);
    plorg();
    void change_plog(int num);
    void show() const;
    

  private:
    char  name[20];
    int IC;
};