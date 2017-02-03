# Trabalho de Implementação de Pilha de Protocolos

[Introdução](#introducao)

[Camada Física](#camada-fisica)

[Camada de Rede](#camada-de-rede)

[Camada de Transporte](#camada-de-transporte)

[Camada de Aplicação](#camada-de-aplicacao)


# Introdução

As camadas implementadas seguem as devidas [especificações](https://docs.google.com/document/preview?hgd=1&id=1O3cNM0T6gFNz9PeMYcnzbmBzEe8J7k34DaefJDSsv4A).

# Funcionamento

Existem 2 pilhas de execução: uma para o cliente e outra para o servidor, como pode ser observado no diagrama abaixo:

```
   CLIENTE                     SERVIDOR

+-----------+               +-----------+
| Aplicação |               | Aplicação |
| (browser) |               |(server.js)|
|           |               |   :7897   |
+-----------+               +-----------+
      |                           |
      |                           |
+------------+               +------------+
|  Transp    |               |  Transp    |
|(client.cpp)|               |(server.cpp)|
|   :7894    |               |   :7896    |
+------------+               +------------+
      |                           |
      |                           |
+-----------+               +-----------+
|    Rede   |               |    Rede   |
|(client.rb)|               |(server.rb)|
|   :7893   |               |   :7895   |
+-----------+               +-----------+
      |                           |
      |                           |
+-----------+               +-----------+
|  Física   |     ----      |  Física   |
|(client.go)|               |(server.go)|
|   :7892   |               |   :7891   |
+-----------+               +-----------+
```

Por decisão de implementação, foi definido a comunicação entre cada camada através de sockets.

Em cada camada implementada, encontramos dois arquivos: um `client` e outro `server`. Os arquivos clientes, são executados na pilha do cliente e os arquivos `server` no servidor.

```
tp-redes/
   application/
      server.js
   transport/
      client.cpp
      server.cpp
   network/
      client.rb
      server.rb
   physical/
      client.go
      server.go
```

Em todas as camadas definimos um fluxo de comunicação. Por exemplo, na pilha cliente, cada camada possui um socket aberto que escuta a camada superior. Quando uma mensagem é recebida no socket, esta mensagem é processada e então é estabelecida uma nova conexão com a camada inferior, que também possui um socket em estado de escuta. A mensagem é enviada e então é aguardada a resposta da camada inferior. Quando recebido a resposta, a resposta é processada e então uma nova mensagem é enviada para a camada superior.

Este fluxo procede para todas as camadas, com a única diferença que na camada do servidor, o fluxo acontece da camada inferior até a mais superior.


# Instalação

## Camada Física - [Go](https://golang.org/)

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

## Camada Rede - [Ruby](https://www.ruby-lang.org/pt/documentation/installation/)

```
sudo apt-get install ruby-full
```


## Camada Transporte - [G++](https://gcc.gnu.org/)

```
sudo apt-get install g++
```

## Camada Aplicação - [Node.js](nodejs.org)

```
curl -sL https://deb.nodesource.com/setup_4.x | sudo -E bash -
sudo apt-get install -y nodejs
```



# Execução

Em dois computadores conectados a mesma rede, deve-se definir um computador para a execução da pilha cliente e outro para a execução da pilha servidor. Feito isso, no respectivo computador execute os seguintes comandos abaixo. Lembre de executar cada camada em um terminal.

## Servidor

Aplicação
```bash
nodejs server.js
```

Transporte
```bash
g++ server.cpp -o server
./server
```
Rede
```bash
ruby server.rb
```
Física
```bash
go run server.go
```
Nese momento, toda a pilha do servidor está pronta para receber uma mensagem da pilha cliente.

## Cliente


Física
```bash
go run client.go
```

Rede
```bash
ruby client.rb
```

Transporte
```bash
g++ client.cpp -o client
./client
```

Aplicação (browser)
```
localhost:7894
```


Ao fazer isto, uma mensagem HTTP será enviada ao servidor. É importante notar que a requisição não acontece diretamente para o servidor nodejs sendo executado. Ao invés disso, a mensagem HTTP é enviada para a camada de transporte do cliente, que encapsula a mensagem em um frame e então envia para a camada de rede que encapsula e envia para a camada física do lado servidor. No lado servidor o frame é decodificado e a mensagem é enviada para a camada de aplicação. A requisição é então processada no servidor e uma mensagem HTTP é enviada ao cliente percorrendo todo o caminho de volta até ser exibida no browser.

Durante a execução é possível ver todo o conteúdo das requisições e das respostas na saída do terminal onde estão sendo executados os programas da camada física.

Todos os detalhes do protocolo HTTP são resolvidos pelo browser e pelo servidor HTTP.  Logo, o que coube a implementação, foi garantir o envio e recebimento das mensagens para as camadas corretas.

Decidimos por rodar as camadas no localhost, como a camada de transporte é em c++, não conseguimos criar uma função para pegar a url, por isso, decimos em colocar o ip manualmente na camada física.

Abaixo podemos conferir o atual esquema de relacionamento entre as camadas que foram implementadas.
