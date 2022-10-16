#ifndef PRODUCTTYPEDAO_H
#define PRODUCTTYPEDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class ProductType;

class ProductTypeDao
{
public:
    explicit ProductTypeDao(QSqlDatabase& db);
    void init() const;

    void add(ProductType& productType) const;
    void update(const ProductType& productType) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<ProductType>>> productTypes() const;

private:
    QSqlDatabase& mDb;
};

#endif // PRODUCTTYPEDAO_H
