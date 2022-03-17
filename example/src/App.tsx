import 'reflect-metadata';
import * as React from 'react';
import {
  StyleSheet,
  Image,
  View,
  Text,
  FlatList,
  ListRenderItemInfo,
} from 'react-native';
// Replace if you want to test the low level API
import { lowLevelInit } from './Database';
// import { typeORMInit } from './Database';
import type { User } from './model/User';
import { Buffer } from 'buffer';

export default function App() {
  let [users, setUsers] = React.useState<User[]>([]);

  React.useEffect(() => {
    lowLevelInit();
    // typeORMInit().then(setUsers);
  }, []);

  return (
    <View style={styles.container}>
      <Text>Quick SQLite tester app</Text>
      <FlatList
        data={users}
        renderItem={(info: ListRenderItemInfo<User>) => {
          return (
            <View
              style={{
                marginHorizontal: 10,
                marginVertical: 4,
                borderRadius: 10,
                padding: 10,
                backgroundColor: 'white',
              }}
            >
              <Image
                style={{ width: 64, height: 64 }}
                source={{
                  uri: `data:image/png;base64,${Buffer.from(
                    info.item.avatar
                  ).toString('base64')}`,
                }}
              />
              <Text style={{ fontWeight: 'bold' }}>Name</Text>
              <Text>{info.item.name}</Text>
              <Text style={{ fontWeight: 'bold', marginTop: 10 }}>Age</Text>
              <Text>{info.item.age}</Text>
              <Text>{info.item.metadata.nickname}</Text>
              {/* <Text style={{ fontWeight: 'bold', marginTop: 10 }}>
                Favorite Book
              </Text>
              <Text>{info.item.favoriteBook.title}</Text> */}
            </View>
          );
        }}
        keyExtractor={(item: any) => item.id}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    paddingTop: 40,
    backgroundColor: '#EEE',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
