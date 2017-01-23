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
go run client.go
```

Execute o servidor:
```
go run server.go
```


## Camada Aplicação

### Introdução

Levando em consideração que todo o processo de instalação da camada física foi realizado, podemos dar continuidade a camada de aplicação.

A camada de aplicação do lado cliente será o browser e no lado do servidor foi implementado um simples servidor HTTP.

### Instalação

Instalação do [nodejs](nodejs.org)

```
curl -sL https://deb.nodesource.com/setup_4.x | sudo -E bash -
sudo apt-get install -y nodejs
```

### Execução

Execute o servidor da camada de aplicação:
```bash
nodejs server.js
```

## Camada Transporte

### Introdução

Levando em consideração que todo o processo de instalação da camada física e aplicação foi realizado, podemos dar continuidade a camada de transporte.

A camada de transporte irá conectar a camada de aplicação e a camada fisica já implementadas anteriormente.

### Instalação

Instalação do [gcc]

```
sudo apt-get install gcc
```

## Camada Rede

### Introdução

Levando em consideração que todo o processo de instalação da camada física, aplicação e transporte foi realizado, podemos dar continuidade a camada de rede.

A camada de conectar irá conectar a camada de transporte e a camada fisica já implementadas anteriormente.

### Instalação

Instalação do [ruby](https://www.ruby-lang.org/pt/documentation/installation/)

```
sudo apt-get install ruby-full
```

### Execução

Execute o servidor da camada de aplicação:
```bash
nodejs server.js
```
Execute o servidor da camada de transporte:
```bash
gcc server1.c -o server
./server
```
Execute o servidor da camada de rede:
```bash
ruby server1.rb
```
Execute o servidor da camada física:
```bash
go run server.go
```
Execute o cliente da camada física:
```bash
go run client.go
```
Execute o cliente da camada de rede:
```bash
ruby server1.rb
```
Execute o cliente da camada de transporte:
```bash
gcc client1.c -o cliente
./server
```

O lado cliente da camada física será executado através de um browser. Para isto basta acessar no browser o endereço:

```
localhost:7894
```
Ao fazer isto, uma mensagem HTTP será enviada ao servidor. É importante notar que a requisição não acontece diretamente para o servidor nodejs sendo executado. Ao invés disso, a mensagem HTTP é enviada para a camada de transporte do cliente, que encapsula a mensagem em um frame e então envia para a camada de rede que encapsula e envia para a camada física do lado servidor. No lado servidor o frame é decodificado e a mensagem é enviada para a camada de aplicação. A requisição é então processada no servidor e uma mensagem HTTP é enviada ao cliente percorrendo todo o caminho de volta até ser exibida no browser.

Durante a execução é possível ver todo o conteúdo das requisições e das respostas na saída do terminal onde estão sendo executados os programas da camada física.

Todos os detalhes do protocolo HTTP são resolvidos pelo browser e pelo servidor HTTP.  Logo, o que coube a implementação, foi garantir o envio e recebimento das mensagens para as camadas corretas.

Abaixo podemos conferir o atual esquema de relacionamento entre as camadas que foram implementadas.

```
   CLIENTE					   SERVIDOR

+-----------+				+-----------+
| Aplicação |				| Aplicação |
| (browser) |				|(server.js)|
+-----------+				+-----------+
	    |								|
	    |								|
+-----------+				+-----------+
|  Transp   |     	|  Transp   |
|(client1.c)|				|(server1.c)|
+-----------+				+-----------+
      |								|
      |								|
+-----------+				+-----------+
|    Rede   |     	|    Rede   |
|(client.rb)|				|(server.rb)|
+-----------+				+-----------+
      |								|
      |								|
+-----------+				+-----------+
|  Física   |  ----	|  Física   |
|(client.go)|				|(server.go)|
+-----------+				+-----------+
```
