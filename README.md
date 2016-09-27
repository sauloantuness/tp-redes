# tp-redes

## Camada Física

### Instalação

Baixar:

```
sudo curl -O https://storage.googleapis.com/golang/go1.7.1.linux-amd64.tar.gz
```

Extrair:

```
sudo tar -xvf go1.7.1.linux-amd64.tar.gz
sudo mv go /usr/local
```

Editar o .profile:

```
sudo nano ~/.profile
```

Adicionar Go ao path adicionando a seguinte linha no .profile:

```
export PATH=$PATH:/usr/local/go/bin
```

Atualizar .profile:

```
source ~/.profile
```

Use o comando `run` para executar o arquivo de teste:

```
go run hello.go
```

Ou compile e execute:

```
go build hello.go
./hello
```

### Executando

Execute o cliente:
```
run client.go
```

Execute o servidor:
```
run server.go
```
