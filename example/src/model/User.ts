import { Book } from './Book'
import {Entity, Column, PrimaryGeneratedColumn, OneToOne, JoinColumn } from "typeorm/browser"

@Entity('user')
export class User {
  @PrimaryGeneratedColumn("uuid")
  id!: string;

  @Column("text")
  name!: string;

  @Column("int")
  age!: number;

  @Column("float")
  networth!: number;

  @OneToOne(() => Book)
  @JoinColumn()
  favoriteBook!: Book
}