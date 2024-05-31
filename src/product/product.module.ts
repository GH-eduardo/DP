import { Module, MiddlewareConsumer, RequestMethod } from '@nestjs/common';
import { ValueMiddleware } from '../middleware/value.middleware';
import { MongooseModule } from "@nestjs/mongoose";
import { ProductService } from './product.service';
import { ProductController } from './product.controller';
import { ProductSchema } from '../schemas/product.schema';

@Module({
    imports: [
        MongooseModule.forFeature([{ name: 'Product', schema: ProductSchema }]),
    ],
    controllers: [ProductController],
    providers: [ProductService],
})
export class ProductModule {
    configure(consumer: MiddlewareConsumer) {
        consumer
            .apply(ValueMiddleware)
            .forRoutes({ path: 'product', method: RequestMethod.ALL });
    }
}