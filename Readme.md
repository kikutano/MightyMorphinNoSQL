<div style="text-align: center;">
    <img src="logo.png" alt="Descrizione" width="160">
</div>

# MightyMorphinNoSQL
This is a **toy projects!** The goal is to create a **NoSQL Database** from scratch, super light, without dependencies just for fun and learning. This database uses **Append-Only Log** paradigm. It's a good idea? No. So why? Why not?!

### File format:
Header:
```
index_counter:1000001
```

Database:
```
[id],[content]

//example
80,{"name":"josh",age:19}
```

### Usage:
You can add rows using this command:
```
main.exe insert <content>
main.exe find <id>
main.exe insert_one_millions (insert one million of rows!)
```

For now find by id can reach the result in **~150ms** in the wrost case scenario. My goal is to create the fastest queries to retrieve result as soon as I can. It's for learning purpose! :D