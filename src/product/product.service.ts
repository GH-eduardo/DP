import { Injectable } from '@nestjs/common';

@Injectable()
export class ProductService {
  private products = [];

  getAll() {
    return this.products;
  }

  getOne(id: number) {
    return this.products.find(product => product.id === id);
  }

  create(product) {
    this.products.push(product);
  }

  update(id: number, product) {
    const index = this.products.findIndex(product => product.id === id);
    this.products[index] = product;
  }

  delete(id: number) {
    this.products = this.products.filter(product => product.id !== id);
  }
}