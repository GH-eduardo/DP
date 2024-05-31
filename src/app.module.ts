import { Module } from '@nestjs/common';
import { ProductModule } from './product/product.module';
import { MongooseModule } from '@nestjs/mongoose';

@Module({
  imports: [
    MongooseModule.forRoot("mongodb://0.0.0.0/product-nest"),
    ProductModule
  ],
  controllers: [],
  providers: [],
})
export class AppModule { }
