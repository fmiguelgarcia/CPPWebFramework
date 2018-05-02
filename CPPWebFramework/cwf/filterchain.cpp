/*
 Copyright 2017 Herik Lima de Castro and Marcelo Medeiros Eler
 Distributed under MIT license, or public domain if desired and
 recognized in your jurisdiction.
 See file LICENSE for detail.
*/

#include "filterchain.h"
#include "configuration.h"
#include "constants.h"

#include <QHash>

CWF_BEGIN_NAMESPACE

FilterChain::FilterChain(Controller *controller, const Configuration &configuration) :  controller(controller), configuration(configuration)
{
}

void FilterChain::doFilter(CWF::Request &request, CWF::Response &response)
{
    if(controller != nullptr)
    {
        using MethodSelector = std::function< void( Controller* controller, Request& request, Response & response)>;
        static const QHash<QByteArray, MethodSelector> methodMapper = {
            {
                HTTP::METHOD::GET(),
                []( Controller* controller, Request& request, Response & response){
                    controller->doGet(request, response);
                }
            },
            {
                HTTP::METHOD::POST(),
                []( Controller* controller, Request& request, Response & response){
                    controller->doPost(request, response);
                }
            },
            {
                HTTP::METHOD::PUT(),
                []( Controller* controller, Request& request, Response & response){
                    controller->doPut(request, response);
                }
            },
            {
                HTTP::METHOD::DELETE(),
                []( Controller* controller, Request& request, Response & response){
                    controller->doDelete(request, response);
                }
            },
            {
                HTTP::METHOD::OPTIONS(),
                []( Controller* controller, Request& request, Response & response){
                    controller->doOptions(request, response);
                }
            },
            {
                HTTP::METHOD::TRACE(),
                []( Controller* controller, Request& request, Response & response){
                    controller->doTrace(request, response);
                }
            }
        };

        const HttpParser &parser = request.getHttpParser();
        const QByteArray method = parser.getMethod();

        auto itr = methodMapper.find( method);
        if( itr != methodMapper.end())
            itr.value()( controller, request, response);
    }
    else
    {
        FileManager fileManager;
        QString url  = request.getRequestURL();
        QString path = request.getPath();
        const QString &extention = fileManager.fileExtention(url);


        if(url == FILE_EXTENTION::BAR())
        {
            request.getRequestDispatcher(configuration.getIndexPage()).forward(request, response);
        }
        else
        {
            const QMimeType pathMime = mMimeDB.mimeTypeForName( path);
            if( pathMime.isValid())
                write(response, path, url, HTTP::CONTENT_TYPE(), pathMime.name().toUtf8());
            else
            {
                response.setStatus(Response::SC_NOT_FOUND, STATUS::NOT_FOUND());
                response.addHeader("Content-Type; charset=UTF-8", "text/html");
                request.getRequestDispatcher(STATUS::STATUS_404()).forward(request, response);
            }
        }
    }
}

void FilterChain::write(Response &response, const QString &path, const QString &url, const QByteArray &name, const QByteArray &value) const
{
    QFile file(path + url);
    if(file.open(QIODevice::ReadOnly))
    {
        response.addHeader(name, value);
        response.write(file.readAll());
    }
    else
    {
        response.sendError(0, file.errorString().toLatin1());
    }
}

CWF_END_NAMESPACE
