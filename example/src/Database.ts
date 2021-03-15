import { createConnection, getRepository } from 'typeorm/browser';
import { Book } from './model/Book';
import { User } from './model/User';

export const createDb = async () => {
  await createConnection({
    type: 'react-native',
    database: 'test',
    location: 'default',
    logging: ['error', 'query', 'schema'],
    synchronize: true,
    entities: [
      User,
      Book
    ]
  });

  // const user1 = new User();
  // user1.name = "Perico de los palotes";
  // user1.age = 30;
  // user1.networth = 30000.23;
  
  // const book1 = new Book();
  // book1.title = "Lord of the ringts"
  // book1.user = user1

  // user1.favoriteBook = book1

  const bookRepository = getRepository(Book)
  const userRepository = getRepository(User)

  // await bookRepository.save(book1)
  // await userRepository.save(user1)


  const users = await userRepository.find();
  
  // const firstUser = users[0];

  // userRepository.remove(firstUser);
  
  return users
}