
// Bibliotecas 
const fs = require('fs');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

const readline = require('readline');


// ! Quando o programa comeca um monte dados com tempo 0 e 1 sao gravados, resolver isso

console.log("Lista de comandos:\n fim(f) -> Termina o programa e gera CSV\n tempo(t) -> Tempo desde inicio \n mostrar(m) -> Mostra no promp os valores que estão sendo lidos\n esconder(e) -> Para de mostrar os dados que estão sendo lidos no promp \n parcial(p) -> Gera o CSV enquanto o teste está acontecendo(programa continua lendo os valores de resistencia)\n \n Para finalizar o programa de forma forçada é necessario clicar Control + C, duas vezes (resultados inesperados)\n\n!!!O programa só começa a aquisitar os dados depois que o nome do arquivo foi definido!!!\n Bom Teste! ")

// Port sendo lido e Frequencia 
const port = new SerialPort('COM5', { baudRate: 115200 });
const parser = port.pipe(new Readline({ delimiter: '-\x00' }));


let nomeArquivo;


// TODO É necessario clicar Control C duas vezes pra matar o programa
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
  });


let fileStream;
// Confere se é a primeira informação que o programa recebe
let primeiraInfo = true;

// Tempo que o programa começou a rodar
let inicioTempo;


// Variavel que define se os dados serão mostrados no promp
let mostrarDados = false;

rl.question('Nome do Arquivo: ', (nomeArquivoInput) => 
{
    nomeArquivo = nomeArquivoInput;

    // Cria stream, para salvar as informações enquanto elas chegam
    // TODO Descobrir o que essa flag faz
    fileStream = fs.createWriteStream(`Resultados (JSON)/${nomeArquivo}.json`, { flags: 'a' });

    inicioTempo = process.hrtime();
    parser.on('data', (data) => 
    {


        //console.log(data)
        if (nomeArquivo != undefined)
        {
            EscreverJSON(data, TempoDesdeInicio())
        }
    });
  
});

//---------------------------------------------------------"Funções"---------------------------------------------------------//
// Tratamento dos inputs que o usuario pode fazer 
rl.on('line', (input) => {
    if (input == "fim" || input == "f") 
    {
        FinalizarJSON();
        EscreverCSV(Finalizar);   
    }
    else if(input == "tempo" || input == "t")
    {
        console.log(`Se passaram ${Math.trunc(TempoDesdeInicio()/1000)}s`);
    }
    else if (input == "mostrar" || input == "m")
    {
        console.log(`Os valores irão aparecer, digite "esconder" ou "e" para que não apareçam`);
        mostrarDados = true;
    }
    else if(input == "esconder" || input == "e")
    {
        console.log(`Os valores não irão aparecer, digite "mostrar" ou "m" para que apareçam`);
        mostrarDados = false;
    }
    else if(input == "parcial" || input == "p")
    {
        EscreverCSV();
    }
    // Caso o nome do arquivo não exista o nome do arquivo é o input que não é um comando. 
    else if(nomeArquivo == undefined)
    {
        nomeArquivo = input;
    }
    else
    {
        console.log(`Comando ${input} não reconhecido :/`);
    }
    });

function TempoDesdeInicio()
{
    const elapsed = process.hrtime(inicioTempo);
    // Todo Descobrir pq essa função é tão complexa
    const elapsedMs = Math.trunc((elapsed[0] * 1000) + (elapsed[1] / 1000000));
    return elapsedMs
}

//TODO Conferir se a pasta existe, senão existir criar ela
function EscreverJSON(data, elapsedMs)
{
    //let jsonData = JSON.parse(`[${JSON.stringify(data)}]`);

    let jsonData = JSON.parse("[" + data.replace(/null/g, '') + "]");


    // Converte a informação que estava [resistencia, resistencia] para [temp, temp]
    jsonData = [jsonData[0], jsonData[1], jsonData[2], jsonData[3]]
    if (mostrarDados)
    {
        console.log(jsonData);
    }

    // Formata para o JSON
    
    const infoFormatada = { tempo: elapsedMs, RPM_Esquerdo: jsonData[0], RPM_Direito: jsonData[1], Torque_Esquerdo: jsonData[2], Torque_Direito: jsonData[3]};

    // Formata o JSON para ficar legivel (uma coisa em cada linha)
    const outputData = JSON.stringify(infoFormatada);
    if (primeiraInfo) 
    {
      fileStream.write(`[${outputData}`);
      primeiraInfo = false;
    } 
    else 
    {
      fileStream.write(`,\n${outputData}`);
    }

}

// Finaliza o JSON fechando a lista com ] e terminar o stream que estava escrevendo o arquivo
function FinalizarJSON()
{
    fileStream.write(']');
    fileStream.end();
}

// TODO Ver se não é melhor escrever CSV ao mesmo tempo que o JSON em vez de depois
function EscreverCSV(CSVEscrito)
{
    fs.readFile(`Resultados (JSON)/${nomeArquivo}.json`, 'ascii', (err, jsonData) => {
        if (err) throw err;

        let data;
      
        // Quando usamos a função parcial (escrever o CSV enquanto o JSON ainda está sendo escrito) ocorre um erro
        // pois o JSON não foi finalizado com o ], esse try-catch cuida disso sem alterar o JSON original
        try
        {
            data = JSON.parse(jsonData);
        }
        catch(error)
        {
            data = JSON.parse(jsonData + "]")
        }
      
        // Cabeçalhos do CSV
        const Cabecalhos = ['Tempo(ms)', 'RPM Esquerdo', 'RMP Direito', 'Torque Esquerdo', 'Torque Direito'];

        // Create an array of CSV rows by mapping over the data object
        const csvRows = data.map((obj) => {
            const tempo = obj.tempo;
            const RPMD = obj.RPM_Direito;
            const RPME = obj.RPM_Esquerdo;
            const TD = obj.Torque_Direito;
            const TE = obj.Torque_Esquerdo;
            return `${tempo},${RPME},${RPMD}, ${TE}, ${TD}`;
        });

        const csvData = Cabecalhos.join(',') + '\n' + csvRows.join('\n');
       
        
        fs.writeFile(`Resultados (CSV)/${nomeArquivo}.csv`, csvData, (err) => {
          if (err) throw err;
          console.log('CSV salvo.');

          // * Confere se existe um callback e o chama quando o CSV foi escrito
          if (CSVEscrito)
          {
            CSVEscrito();
          };
          return true
        });
      });
}

// Quando a conexão no PORT é fechada forçadamente essa função garante que o .json encerre corretamente
function FinalizarForcado() {
    console.log("Conexão Finalizada forçadamente, gere CSV pelo outro arquivo")
    FinalizarJSON();
    Finalizar();
  }

// Finaliza a leitura do port e promp
function Finalizar()
{
    port.close();
    process.exit();
}

// Quando a conexão no PORT é fechada essa função garante que o arquivo .json encerre corretamente 
port.on('close', () => {
    FinalizarJSON();
  });

// Chama a função FinalizarForcado quando control c é clicado
process.on('SIGINT', FinalizarForcado);




