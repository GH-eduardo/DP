import { Controller, Get, Post, Put, Delete, Param, Body } from '@nestjs/common';
import { ProductService } from './product.service';

@Controller('product')
export class ProductController {
  constructor(private readonly productService: ProductService) {}

  @Get()
  getAll(): any[] {
    return this.productService.getAll();
  }

  @Get(':id')
  getOne(@Param('id') id: number): any {
    return this.productService.getOne(id);
  }

  @Post()
  create(@Body() product: any): void {
    this.productService.create(product);
  }

  @Put(':id')
  update(@Param('id') id: number, @Body() product: any): void {
    this.productService.update(id, product);
  }

  @Delete(':id')
  delete(@Param('id') id: number): void {
    this.productService.delete(id);
  }
}