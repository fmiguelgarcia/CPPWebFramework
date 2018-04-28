#include "httpcookie.h"
#include "constants.h"

#include <QHash>
#include <functional>

namespace
{

#if defined Q_COMPILER_CONSTEXPR
	Q_DECL_CONSTEXPR int maxInt64StrSize() Q_DECL_NOEXCEPT
	{
		Q_CONSTEXPR int sz = strlen( "+9223372036854775807");
		return sz;
	}

	Q_DECL_CONSTEXPR  int maxFixedFields() Q_DECL_NOEXCEPT
	{
		Q_CONSTEXPR int sz = strlen( "=; Comment=; Domain=; Max-Age=; Path=; Secure; Version=");
		return sz;
	}
#else
	inline int maxInt64StrSize() Q_DECL_NOEXCEPT
	{ return 20; };

	inline int maxFixedFields() Q_DECL_NOEXCEPT
	{ return 55; }
#endif
}

CWF_BEGIN_NAMESPACE

HttpCookie::HttpCookie(const QByteArray &source)
{
	using CookieHandler = std::function< void( HttpCookie*, const QByteArray&)>;
	static const QHash<QString, CookieHandler> cookieHandler= {
		{
			COOKIE::COMMENT(),
			[]( HttpCookie* target, const QByteArray& value){
				target->comment = value;
			}
		},
		{
			COOKIE::DOMAIN_COOKIE(),
			[]( HttpCookie* target, const QByteArray& value){
				target->domain = value;
			}
		},
		{
			COOKIE::MAX_AGE(),
			[]( HttpCookie* target, const QByteArray& value){
				target->maxAge = value.toInt();
			}
		},
		{
			COOKIE::PATH(),
			[]( HttpCookie* target, const QByteArray& value){
				target->path = value;
			}
		},
		{
			COOKIE::SECURE(),
			[]( HttpCookie* target, const QByteArray& ){
				target->secure = true;
			}
		},
		{
			COOKIE::VERSION(),
			[]( HttpCookie* target, const QByteArray& value){
				target->version = value.toInt();
			}
		}
	};

   QByteArray name;
   QByteArray value;

   QList<QByteArray> list(std::move(splitCSV(source)));
   for(QByteArray &current : list)
   {
       const int position = current.indexOf('=');
       if(position)
       {
           name  = current.left(position).trimmed();
           value = current.mid(position + 1).trimmed();
       }
       else
       {
           name = current.trimmed();
           value.clear();
       }

		auto itr = cookieHandler.find( name);
		if( itr != cookieHandler.end())
			itr.value()( this, value);
       else
       {
           if (this->name.isEmpty())
           {
               this->name  = name;
               this->value = value;
           }
           else
           {
               qDebug() << "HttpCookie: Ignoring unknown " << name.data() << "=" << value.data();
           }
       }
   }
}

HttpCookie::HttpCookie(const QByteArray &name, const QByteArray &value) : name(name), value(value)
{
}

QByteArray HttpCookie::toByteArray() const
{
   QByteArray buffer;

	/// @note Optimization to avoid multiples reallocations.
	const int maxSize =
		// Variable part
		name.size() + value.size() + comment.size() + domain.size() + path.size() +
		// maxAge & version
		maxInt64StrSize() + maxInt64StrSize() +
		// Const part
		maxFixedFields();
	buffer.reserve( maxSize);

   buffer.append( name);
   buffer.append('=');
   buffer.append(value);
   if(!comment.isEmpty())
   {
       buffer.append("; Comment=");
       buffer.append(comment);
   }
   if(!domain.isEmpty())
   {
       buffer.append("; Domain=");
       buffer.append(domain);
   }
   if(maxAge != 0)
   {
       buffer.append("; Max-Age=");
       buffer.append(QByteArray::number(maxAge));
   }
   if(!path.isEmpty())
   {
       buffer.append("; Path=");
       buffer.append(path);
   }
   if(secure)
   {
       buffer.append("; Secure");
   }
   buffer.append("; Version=");
   buffer.append(QByteArray::number(version));
   return buffer;
}

QList<QByteArray> HttpCookie::splitCSV(const QByteArray &source)
{
   bool inString = false;
   QList<QByteArray> list;
   QByteArray buffer;
   int size = source.size();
   for(int i = 0; i < size; ++i)
   {
       char c = source.at(i);
       if(!inString)
       {
           if(c == '\"')
           {
               inString = true;
           }
           else if(c == ';')
           {
               QByteArray trimmed(std::move(buffer.trimmed()));
               if (!trimmed.isEmpty())
               {
                   list.append(trimmed);
               }
               buffer.clear();
           }
           else
           {
               buffer.append(c);
           }
       }
       else if(c == '\"')
       {
           inString = false;
       }
       else
       {
           buffer.append(c);
       }
   }
   QByteArray trimmed(std::move(buffer.trimmed()));
   if(!trimmed.isEmpty())
   {
       list.append(trimmed);
   }
   return list;
}

CWF_END_NAMESPACE
